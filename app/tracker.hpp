// Copyright 2022 Leonhard S.

#pragma once

#include <memory>

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>

#include "arx.hpp"
#include "ps2.hpp"

#include "game/character-info.hpp"
#include "game/state.hpp"

namespace PresenceLib {
class EssClient;
} // namespace PresenceLib

namespace PresenceApp {

class ActivityTracker : public QObject {
    Q_OBJECT

public:
    explicit ActivityTracker(
        const CharacterData& character,
        QObject* parent = nullptr);
    ~ActivityTracker() override;
    ActivityTracker(const ActivityTracker& other) = delete;
    ActivityTracker(ActivityTracker&& other) noexcept = delete;
    ActivityTracker& operator=(const ActivityTracker& other) = delete;
    ActivityTracker& operator=(ActivityTracker&& other) noexcept = delete;

    CharacterData getCharacter() const;

Q_SIGNALS:
    void ready();
    void stateChanged(GameState state);
    void payloadReceived(const QString& event_name,
        const QJsonObject& payload);

private Q_SLOTS:
    void onPayloadReceived(const QString& event_name,
        const QJsonObject& payload);

private:
    QList<QJsonObject> generateSubscriptions() const;
    void handleDeathPayload(const QJsonObject& payload);
    void handleGainexperiencePayload(const QJsonObject& payload);

    CharacterData character_;
    GameStateFactory state_factory_;
    GameState current_state_;
    std::unique_ptr<PresenceLib::EssClient> ess_client_;
};

} // namespace PresenceApp
