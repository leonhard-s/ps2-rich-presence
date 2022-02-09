// Copyright 2022 Leonhard S.

#ifndef PS2RPC_PRESENCE_HANDLER_HPP
#define PS2RPC_PRESENCE_HANDLER_HPP

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include "discord.h"

namespace ps2rpc
{

    class PresenceHandler : public QObject
    {
        Q_OBJECT

    public:
        explicit PresenceHandler(QObject *parent = nullptr);

        // Q_SIGNALS:

    public Q_SLOTS:
        void setActivity(discord::Activity activity);

    private:
        discord::Core *discord_core_;
        QTimer *timer_;
    };

} // namespace ps2rpc

#endif // PS2RPC_PRESENCE_HANDLER_HPP
