// Copyright 2022 Leonhard S.

#pragma once

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <QtCore/QString>

#include "arx.hpp"
#include "ps2.hpp"

#include "ess-client.hpp"
#include "game/character-info.hpp"
#include "game/state.hpp"

namespace PresenceApp {

class ActivityTracker: public QObject {
    Q_OBJECT

public:
    explicit ActivityTracker(
        const CharacterData& character,
        QObject* parent = nullptr);
    ActivityTracker(const ActivityTracker& other) = delete;
    ActivityTracker(ActivityTracker&& other) noexcept = delete;

    ActivityTracker& operator=(const ActivityTracker& other) = delete;
    ActivityTracker& operator=(ActivityTracker&& other) noexcept = delete;

    CharacterData getCharacter() const;

Q_SIGNALS:
    void ready();
    void stateChanged(GameState state);
    void payloadReceived(const QString& event_name,
        const arx::json_t& payload);

private Q_SLOTS:
    void onPayloadReceived(const QString& event_name,
        const arx::json_t& payload);

private:
    QList<arx::Subscription> generateSubscriptions() const;
    void handleDeathPayload(const arx::json_t& payload);
    void handleGainexperiencePayload(const arx::json_t& payload);

    CharacterData character_;
    GameStateFactory state_factory_;
    GameState current_state_;
    QScopedPointer<EssClient> ess_client_;
};

} // namespace PresenceApp
