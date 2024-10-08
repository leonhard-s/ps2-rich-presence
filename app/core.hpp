// Copyright 2022 Leonhard S.

#pragma once

#include <QtCore/QDateTime>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <QtCore/QString>
#include <QtCore/QTimer>

#include "arx.hpp"

#include "game/character-info.hpp"
#include "presence/factory.hpp"
#include "presence/handler.hpp"
#include "tracker.hpp"

namespace PresenceApp {

class RichPresenceApp: public QObject {
    Q_OBJECT

public:
    explicit RichPresenceApp(QObject* parent = nullptr);
    RichPresenceApp(const RichPresenceApp&) = delete;
    RichPresenceApp(RichPresenceApp&&) = delete;

    RichPresenceApp& operator=(const RichPresenceApp&) = delete;
    RichPresenceApp& operator=(RichPresenceApp&&) = delete;

    bool getRichPresenceEnabled() const;
    void setRichPresenceEnabled(bool enabled);
    const CharacterData& getCharacter() const;
    void setCharacter(const CharacterData& character);

    QDateTime getLastEventPayload() const;
    QDateTime getLastGameStateUpdate() const;
    QDateTime getLastPresenceUpdate() const;
    qint32 getEventLatency() const;
    double getEventFrequency();

Q_SIGNALS:
    void characterChanged(const CharacterData& character);
    void eventPayloadReceived();
    void gameStateChanged();
    void presenceUpdated();

private Q_SLOTS:
    void onEventPayloadReceived(
        const QString& event_name,
        const arx::json_t& payload);
    void onGameStateChanged(const GameState& state);
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
    qint32 event_latency_;
    QScopedPointer<ActivityTracker> tracker_;

    QDateTime last_event_payload_;
    QDateTime last_game_state_update_;
    QDateTime last_presence_update_;
    QList<QDateTime> recent_events_;
};

} // namespace PresenceApp
