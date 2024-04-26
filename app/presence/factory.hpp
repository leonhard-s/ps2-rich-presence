// Copyright 2022 Leonhard S.

#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>

#include "discord-game-sdk/discord.h"

#include "game/state.hpp"

namespace PresenceApp {

class PresenceFactory : public QObject {
    Q_OBJECT

public:
    explicit PresenceFactory(QObject* parent = nullptr);
    PresenceFactory(const PresenceFactory& other) = delete;
    PresenceFactory(PresenceFactory&& other) noexcept = delete;

    PresenceFactory& operator=(const PresenceFactory& other) = delete;
    PresenceFactory& operator=(PresenceFactory&& other) noexcept = delete;

    discord::Activity getPresenceAsActivity();

Q_SIGNALS:
    void activityChanged(discord::Activity activity);

public Q_SLOTS:
    void setActivityFromGameState(const GameState& state);
    void setActivityIdle();

private:
    static discord::Activity buildIdleActivity();
    static discord::Activity buildGameActivity(const GameState& state);

    GameState state_;
    bool is_idle_;
};

} // namespace PresenceApp
