// Copyright 2022 Leonhard S.

#include "presence/handler.hpp"

#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTimer>

#include "discord-game-sdk/discord.h"

#include "appdata/appid.hpp"
#include "moc_macros.hpp"

namespace PresenceApp {

PresenceHandler::PresenceHandler(QObject* parent)
    : QObject{ parent }
{
    // Create discord core
    discord::Core* temp_;
    auto result = discord::Core::Create(appid, DiscordCreateFlags_Default, &temp_);
    if (!temp_) {
        qCritical() << "Failed to create discord core (error code" << static_cast<int>(result) << ")";
    }
    else {
        discord_core_.reset(temp_);
    }

    // Configure logging
    discord_core_->SetLogHook(
        discord::LogLevel::Debug, [](discord::LogLevel level, const char* message) { qDebug() << "Discord: " << static_cast<int>(level) << ": " << message; });

    // Create presence update timer
    timer_ = new QTimer(this);
    timer_->setInterval(16); // ~60 FPS
    QObject::connect(timer_, &QTimer::timeout, [this]() { discord_core_->RunCallbacks(); });
    timer_->start();
}

void PresenceHandler::clearActivity() {
    discord_core_->ActivityManager().ClearActivity(
        [](discord::Result result) { qDebug() << ((result == discord::Result::Ok) ? "Succeeded" : "Failed")
        << "clearing activity!"; });
}

void PresenceHandler::setActivity(discord::Activity activity) {
    discord_core_->ActivityManager().UpdateActivity(activity,
        [](discord::Result result) {
            qDebug()
                << ((result == discord::Result::Ok) ? "Succeeded" : "Failed")
                << "updating activity!"; });
}

} // namespace PresenceApp

PUSH_MOC_WARNINGS_FILTER;
#include "moc_handler.cpp"
POP_MOC_WARNINGS_FILTER;
