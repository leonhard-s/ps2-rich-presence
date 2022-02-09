// Copyright 2022 Leonhard S.

#ifndef PS2RPC_PRESENCE_FACTORY_HPP
#define PS2RPC_PRESENCE_FACTORY_HPP

#include <QtCore/QObject>
#include <QtCore/QString>

#include "discord.h"

#include "game/state.hpp"

namespace ps2rpc
{

    class PresenceFactory : public QObject
    {
        Q_OBJECT

    public:
        PresenceFactory(QObject *parent = nullptr);

        discord::Activity getPresenceAsActivity();

    Q_SIGNALS:
        void activityChanged(discord::Activity activity);

    public Q_SLOTS:
        void setActivityFromGameState(const GameState &state);
        void setActivityIdle();

    private:
        discord::Activity buildIdleActivity();
        discord::Activity buildGameActivity(const GameState &state);

        GameState state_;
        bool is_idle_;
    };

} // namespace ps2rpc

#endif // PS2RPC_PRESENCE_FACTORY_HPP
