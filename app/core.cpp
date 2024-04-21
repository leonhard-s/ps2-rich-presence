// Copyright 2022 Leonhard S.

#include "core.hpp"

#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QJsonObject>

#include <algorithm>

#include "arx.hpp"
#include "discord-game-sdk/discord.h"

#include "game/character-info.hpp"
#include "game/state.hpp"
#include "presence/handler.hpp"
#include "tracker.hpp"
#include "moc_macros.hpp"

namespace PresenceApp {

RichPresenceApp::RichPresenceApp(QObject* parent)
    : QObject(parent)
    , rate_limit_timer_{ std::make_unique<QTimer>(this) }
    , presence_enabled_{ true }
    , presence_{ std::make_unique<PresenceFactory>(this) }
    , discord_{ std::make_unique<PresenceHandler>(this) }
    , event_latency_{ -1 }
    , last_event_payload_{ QDateTime::fromSecsSinceEpoch(0) }
    , last_game_state_update_{ QDateTime::fromSecsSinceEpoch(0) }
    , last_presence_update_{ QDateTime::fromSecsSinceEpoch(0) }
{
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
            tracker_ = std::make_unique<ActivityTracker>(character, this);
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
    const auto oldest_event = recent_events_.front();
    const auto now = QDateTime::currentDateTimeUtc();
    const auto timespan = static_cast<double>(oldest_event.secsTo(now));
    auto freq = static_cast<double>(recent_events_.size()) / timespan;
    return freq;
}

void RichPresenceApp::onEventPayloadReceived(
    const QString& event_name,
    const QJsonObject& payload
) {
    auto it = payload.find("timestamp");
    if (it == payload.end()) {
        qWarning() << "No timestamp found for" << event_name << "payload";
        return;
    }
    auto event_time = QDateTime::fromSecsSinceEpoch(
        it->toString().toLong(), Qt::TimeSpec::UTC);
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
    constexpr qint64 recency_threshold = 30000;
    // Remove events older than 30 seconds from the list
    QList<QDateTime> still_fresh;
    auto now = QDateTime::currentDateTimeUtc();
    std::copy_if(recent_events_.begin(), recent_events_.end(),
        std::back_inserter(still_fresh),
        [now](const QDateTime& event_time) {
            return event_time.msecsTo(now) <= recency_threshold;
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
        0LL, static_cast<qint64>(INT_MAX)));
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
    constexpr qsizetype num_recent_events = 100;
    auto now = QDateTime::currentDateTimeUtc();
    // Only keep the 100 most recent events
    while (recent_events_.size() > num_recent_events) {
        recent_events_.pop_front();
    }
    // Add the new event to the list
    recent_events_.push_back(now);
}

} // namespace PresenceApp

PUSH_MOC_WARNINGS_FILTER;
#include "moc_core.cpp"
POP_MOC_WARNINGS_FILTER;
