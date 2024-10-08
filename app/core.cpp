// Copyright 2022 Leonhard S.

#include "core.hpp"

#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTimer>

#include <algorithm>
#include <limits>

#include "arx.hpp"
#include "discord-game-sdk/discord.h"

#include "game/character-info.hpp"
#include "game/state.hpp"
#include "presence/handler.hpp"
#include "tracker.hpp"

namespace PresenceApp {

RichPresenceApp::RichPresenceApp(QObject* parent)
    : QObject(parent)
    , presence_enabled_{ true }
    , event_latency_{ -1 }
{
    presence_.reset(new PresenceFactory(this));
    discord_.reset(new PresenceHandler(this));
    // Reset timestamps
    last_event_payload_ = QDateTime::fromSecsSinceEpoch(0);
    last_game_state_update_ = QDateTime::fromSecsSinceEpoch(0);
    last_presence_update_ = QDateTime::fromSecsSinceEpoch(0);
    // Set up rate limiting timer
    rate_limit_timer_.reset(new QTimer(this));
    rate_limit_timer_->setSingleShot(true);
    rate_limit_timer_->start(0);
    QObject::connect(rate_limit_timer_.get(), &QTimer::timeout,
        this, &RichPresenceApp::onRateLimitTimerExpired);
}

bool RichPresenceApp::getRichPresenceEnabled() const {
    return presence_enabled_;
}

void RichPresenceApp::setRichPresenceEnabled(bool enabled) {
    // If presence was disabled, queue a presence update
    if (!presence_enabled_) {
        schedulePresenceUpdate();
    }
    presence_enabled_ = enabled;
}

const CharacterData& RichPresenceApp::getCharacter() const {
    return character_;
}

void RichPresenceApp::setCharacter(const CharacterData& character) {
    if (character_ != character) {
        character_ = character;
        if (character.id_ != 0) {
            tracker_.reset(new ActivityTracker(character, this));
            QObject::connect(tracker_.get(), &ActivityTracker::payloadReceived,
                this, &RichPresenceApp::onEventPayloadReceived);
            QObject::connect(tracker_.get(), &ActivityTracker::stateChanged,
                this, &RichPresenceApp::onGameStateChanged);
        }
        else {
            tracker_.reset();
        }
        // Reset timestamps and payload cache
        last_event_payload_ = QDateTime::fromSecsSinceEpoch(0);
        last_game_state_update_ = QDateTime::fromSecsSinceEpoch(0);
        event_latency_ = -1;
        recent_events_.clear();
        emit eventPayloadReceived();
        emit gameStateChanged();
        emit characterChanged(character_);
    }
}

QDateTime RichPresenceApp::getLastEventPayload() const {
    return last_event_payload_;
}

QDateTime RichPresenceApp::getLastGameStateUpdate() const {
    return last_game_state_update_;
}

QDateTime RichPresenceApp::getLastPresenceUpdate() const {
    return last_presence_update_;
}

qint32 RichPresenceApp::getEventLatency() const {
    return event_latency_;
}

double RichPresenceApp::getEventFrequency() {
    pruneRecentEvents();
    if (recent_events_.empty()) {
        return 0.0;
    }
    auto oldest_event = recent_events_.front();
    auto now = QDateTime::currentDateTimeUtc();
    double timespan = static_cast<double>(oldest_event.secsTo(now));
    auto freq = static_cast<double>(recent_events_.size()) / timespan;
    return freq;
}

void RichPresenceApp::onEventPayloadReceived(
    const QString& event_name,
    const arx::json_t& payload
) {
    // The app only cares about if there are messages coming in. Handling
    // the payloads and dealing with error states is the tracker's problem.
    Q_UNUSED(event_name)

    // Get timestamp of the event
    auto it = payload.find("timestamp");
    if (it == payload.end()) {
        qWarning() << "No timestamp found for" << event_name << "payload";
        return;
    }
    auto timestamp = QString::fromStdString(
        it.value().get<arx::json_string_t>());
    auto event_time = QDateTime::fromSecsSinceEpoch(
        timestamp.toLong(), Qt::TimeSpec::UTC);
    auto now = QDateTime::currentDateTimeUtc();
    event_latency_ = static_cast<qint32>(event_time.msecsTo(now));
    // Update recent events list; used for event frequency calculation
    last_event_payload_ = now;
    updateRecentEventsList();
    emit eventPayloadReceived();
}

void RichPresenceApp::onGameStateChanged(const GameState& state) {
    // Update the presence factory with the new game state
    presence_->setActivityFromGameState(state);
    emit gameStateChanged();
    schedulePresenceUpdate();
}

void RichPresenceApp::onRateLimitTimerExpired() {
    updatePresence();
}

void RichPresenceApp::pruneRecentEvents() {
    // Remove events older than 30 seconds from the list
    QList<QDateTime> still_fresh;
    auto now = QDateTime::currentDateTimeUtc();
    std::copy_if(recent_events_.begin(), recent_events_.end(),
        std::back_inserter(still_fresh),
        [now](const QDateTime& event_time) {
            return event_time.msecsTo(now) <= 30000;
        });
}

void RichPresenceApp::schedulePresenceUpdate() {
    auto rate_limit = PresenceHandler::PRESENCE_UPDATE_RATE_LIMIT;
    // If it has been longer than the rate limit since the last presence
    // update, update the presence immediately
    auto now = QDateTime::currentDateTimeUtc();
    if (last_presence_update_.msecsTo(now) > rate_limit) {
        qDebug() << "Scheduling immediate presence update";
        updatePresence();
        return;
    }
    // We are not allowed to update the presence again yet. If there is no
    // timer running yet, start one counting down to the next update slot.
    // Any game state changes will still be passed on to the factory in the
    // meanwhile, so the next update will be the most up-to-date one.
    if (rate_limit_timer_->isActive()) {
        qDebug() << "Presence update timer is already running";
        return;
    }
    // Calculate the number of seconds until we ar eallowed to update the
    // presence again.
    auto ms_until_next_update = static_cast<int>(std::clamp(
        rate_limit - last_presence_update_.msecsTo(now),
        0LL, static_cast<long long>(INT_MAX)));
    qDebug() << "Scheduling presence update in" << ms_until_next_update << "ms";
    // (Re)start the timer. Presence will be updated when the timer fires.
    rate_limit_timer_->start(ms_until_next_update);
}

void RichPresenceApp::updatePresence() {
    last_presence_update_ = QDateTime::currentDateTimeUtc();
    if (presence_enabled_) {
        auto activity = presence_->getPresenceAsActivity();
        discord_->setActivity(activity);
    }
    if (!presence_enabled_) {
        discord_->clearActivity();
    }
    emit presenceUpdated();
}

void RichPresenceApp::updateRecentEventsList() {
    auto now = QDateTime::currentDateTimeUtc();
    // Only keep the 100 most recent events
    while (recent_events_.size() > 100) {
        recent_events_.pop_front();
    }
    // Add the new event to the list
    recent_events_.push_back(now);
}

} // namespace PresenceApp

#if defined(_MSC_VER) && !defined(__clang__)
#   pragma warning(push)
#   pragma warning(disable : 4464)
#elif defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wreserved-identifier"
#endif

#include "moc_core.cpp"

#if defined(_MSC_VER) && !defined(__clang__)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif
