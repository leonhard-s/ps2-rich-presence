// Copyright 2022 Leonhard S.

#include "presence/handler.hpp"

#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTimer>

#include "discord.h"

#include "appdata/appid.hpp"

namespace ps2rpc {

PresenceHandler::PresenceHandler(QObject* parent)
    : QObject{ parent }
{
    // Create discord core
    auto result = discord::Core::Create(appid, DiscordCreateFlags_Default, &discord_core_);
    if (!discord_core_)
    {
        qCritical() << "Failed to create discord core (error code" << static_cast<int>(result) << ")";
    }

    // Configure logging
    discord_core_->SetLogHook(
        discord::LogLevel::Debug, [](discord::LogLevel level, const char* message)
        { qDebug() << "Discord: " << static_cast<int>(level) << ": " << message; });

    // Create presence update timer
    timer_ = new QTimer(this);
    timer_->setInterval(16); // ~60 FPS
    QObject::connect(timer_, &QTimer::timeout, [this]()
        { discord_core_->RunCallbacks(); });
    timer_->start();
}

void PresenceHandler::clearActivity()
{
    discord_core_->ActivityManager().ClearActivity(
        [](discord::Result result)
        { qDebug() << ((result == discord::Result::Ok) ? "Succeeded" : "Failed")
        << "clearing activity!"; });
}

void PresenceHandler::setActivity(discord::Activity activity)
{
    discord_core_->ActivityManager().UpdateActivity(
        activity, [](discord::Result result)
        { qDebug() << ((result == discord::Result::Ok) ? "Succeeded" : "Failed")
        << "updating activity!"; });
}

} // namespace ps2rpc
