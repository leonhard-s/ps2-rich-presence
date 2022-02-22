// Copyright 2022 Leonhard S.

#include "core.hpp"

#include <QtCore/QDateTime>
#include <QtCore/QJsonObject>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTimer>

#include "discord.h"

#include "game/character-info.hpp"
#include "game/state.hpp"
#include "presence/handler.hpp"

namespace ps2rpc
{

    RichPresenceApp::RichPresenceApp(QObject *parent)
        : QObject(parent), presence_enabled_{true}
    {
        // Set up rate limiting timer
        rate_limit_timer_.reset(new QTimer(this));
        rate_limit_timer_->setInterval(0); // ASAP
        rate_limit_timer_->setSingleShot(true);
        rate_limit_timer_->start();
        QObject::connect(rate_limit_timer_.get(), &QTimer::timeout,
                         this, &RichPresenceApp::onRateLimitTimerExpired);
    }

    bool RichPresenceApp::getRichPresenceEnabled() const
    {
        return presence_enabled_;
    }

    void RichPresenceApp::setRichPresenceEnabled(bool enabled)
    {
        // If presence was disabled, queue a presence update
        if (!presence_enabled_)
        {
            schedulePresenceUpdate();
        }
        presence_enabled_ = enabled;
    }

    CharacterData RichPresenceApp::getCharacter() const
    {
        return character_;
    }

    void RichPresenceApp::setCharacter(const CharacterData &character)
    {
        if (character_ != character)
        {
            character_ = character;
            emit characterChanged(character_);
        }
    }

    QDateTime RichPresenceApp::getLastEventPayload() const
    {
        return last_event_payload_;
    }

    QDateTime RichPresenceApp::getLastGameStateUpdate() const
    {
        return last_game_state_update_;
    }

    QDateTime RichPresenceApp::getLastPresenceUpdate() const
    {
        return last_presence_update_;
    }

    void RichPresenceApp::onEventPayloadReceived(const QString &event_name,
                                                 const QJsonObject &payload)
    {
        // The app only cares about if there are messages coming in. Handling
        // the payloads and dealing with error states is the tracker's problem.
        Q_UNUSED(event_name);
        Q_UNUSED(payload);

        last_event_payload_ = QDateTime::currentDateTime();
        emit eventPayloadReceived();
    }

    void RichPresenceApp::onGameStateChanged(const GameState &state)
    {
        // Update the presence factory with the new game state
        presence_->setActivityFromGameState(state);
        emit gameStateChanged();
        schedulePresenceUpdate();
    }

    void RichPresenceApp::onRateLimitTimerExpired()
    {
        updatePresence();
    }

    void RichPresenceApp::schedulePresenceUpdate()
    {
        auto rate_limit = PresenceHandler::PRESENCE_UPDATE_RATE_LIMIT;
        // If it has been longer than the rate limit since the last presence
        // update, update the presence immediately
        auto now = QDateTime::currentDateTime();
        if (now.msecsTo(last_presence_update_) > rate_limit)
        {
            updatePresence();
            return;
        }
        // We are not allowed to update the presence again yet. If there is no
        // timer running yet, start one counting down to the next update slot.
        // Any game state changes will still be passed on to the factory in the
        // meanwhile, so the next update will be the most up-to-date one.
        if (rate_limit_timer_->isActive())
        {
            return;
        }
        // Calculate the number of seconds until we ar eallowed to update the
        // presence again.
        auto ms_until_next_update =
            rate_limit - now.msecsTo(last_presence_update_);
        // (Re)start the timer. Presence will be updated when the timer fires.
        rate_limit_timer_->start(ms_until_next_update);
    }

    void RichPresenceApp::updatePresence()
    {
        last_presence_update_ = QDateTime::currentDateTime();
        if (presence_enabled_)
        {
            auto activity = presence_->getPresenceAsActivity();
            discord_->setActivity(activity);
        }
        if (!presence_enabled_)
        {
            discord_->clearActivity();
        }
        emit presenceUpdated();
    }

} // namespace ps2rpc
