// Copyright 2022 Leonhard S.

#ifndef PS2RPC_CORE_HPP
#define PS2RPC_CORE_HPP

#include <QtCore/QDateTime>
#include <QtCore/QJsonObject>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <QtCore/QString>
#include <QtCore/QTimer>

#include "game/character-info.hpp"
#include "presence/factory.hpp"
#include "presence/handler.hpp"
#include "tracker.hpp"

namespace ps2rpc
{

    class RichPresenceApp : public QObject
    {
        Q_OBJECT

    public:
        explicit RichPresenceApp(QObject *parent = nullptr);

        bool getRichPresenceEnabled() const;
        void setRichPresenceEnabled(bool enabled);
        CharacterData getCharacter() const;
        void setCharacter(const CharacterData &character);

        QDateTime getLastEventPayload() const;
        QDateTime getLastGameStateUpdate() const;
        QDateTime getLastPresenceUpdate() const;
        int getEventLatency() const;
        double getEventFrequency();

    Q_SIGNALS:
        void characterChanged(const CharacterData &character);
        void eventPayloadReceived();
        void gameStateChanged();
        void presenceUpdated();

    private Q_SLOTS:
        void onEventPayloadReceived(const QString &event_name,
                                    const QJsonObject &payload);
        void onGameStateChanged(const GameState &state);
        void onRateLimitTimerExpired();

    private:
        void pruneRecentEvents();
        void schedulePresenceUpdate();
        void updatePresence();
        void updateRecentEventsList();

        QScopedPointer<QTimer> rate_limit_timer_;
        CharacterData character_;
        bool presence_enabled_;
        QScopedPointer<PresenceFactory> presence_;
        QScopedPointer<PresenceHandler> discord_;
        double event_latency_;
        QScopedPointer<ActivityTracker> tracker_;

        QDateTime last_event_payload_;
        QDateTime last_game_state_update_;
        QDateTime last_presence_update_;
        QList<QDateTime> recent_events_;
    };

} // namespace ps2rpc

#endif // PS2RPC_CORE_HPP
