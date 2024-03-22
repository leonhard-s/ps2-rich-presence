// Copyright 2022 Leonhard S.

#pragma once

#include <memory>

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include "discord-game-sdk/discord.h"

namespace PresenceApp {

class PresenceHandler : public QObject {
    Q_OBJECT

public:
    static constexpr qint16 PRESENCE_UPDATE_RATE_LIMIT = 15000;

    explicit PresenceHandler(QObject* parent = nullptr);
    PresenceHandler(const PresenceHandler& other) = delete;
    PresenceHandler(PresenceHandler&& other) noexcept = delete;

    PresenceHandler& operator=(const PresenceHandler& other) = delete;
    PresenceHandler& operator=(PresenceHandler&& other) noexcept = delete;

public Q_SLOTS:
    void clearActivity();
    void setActivity(discord::Activity activity);

private:
    std::unique_ptr<discord::Core> discord_core_;
    QTimer* timer_;
};

} // namespace PresenceApp
