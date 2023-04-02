// Copyright 2022 Leonhard S.

#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include "discord.h"

namespace ps2rpc {

class PresenceHandler: public QObject
{
    Q_OBJECT

public:
    static constexpr qint16 PRESENCE_UPDATE_RATE_LIMIT = 15000;

    explicit PresenceHandler(QObject* parent = nullptr);

public Q_SLOTS:
    void clearActivity();
    void setActivity(discord::Activity activity);

private:
    discord::Core* discord_core_;
    QTimer* timer_;
};

} // namespace ps2rpc
