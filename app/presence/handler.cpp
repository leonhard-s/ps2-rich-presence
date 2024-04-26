// Copyright 2022 Leonhard S.

#include "handler.hpp"

#include <QDebug>
#include <QObject>
#include <QTimer>
#include <discord-game-sdk/discord.h>
#include <adopt_pointer.h>
#include <moc_macros.h>

#include "appdata/appid.hpp"

namespace PresenceApp {

PresenceHandler::PresenceHandler(QObject* parent)
    : QObject{ parent }
{
    // Create discord core
    discord::Core* temp_ = nullptr;
    auto result = discord::Core::Create(appid, DiscordCreateFlags_Default, &temp_);
    if (temp_ == nullptr) {
        qCritical() << "Failed to create discord core (error code" << static_cast<int>(result) << ")";
    }
    else {
        discord_core_.reset(temp_);
    }

    // Configure logging
    discord_core_->SetLogHook(
        discord::LogLevel::Debug, [](discord::LogLevel level, const char* message) { qDebug() << "Discord: " << static_cast<int>(level) << ": " << message; });

    // Create presence update timer
    timer_ = adopt_pointer(new QTimer(this));
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
