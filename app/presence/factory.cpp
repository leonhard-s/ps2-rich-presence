// Copyright 2022 Leonhard S.

#include "presence/factory.hpp"

#include <QtCore/QObject>
#include <string>

#include "discord-game-sdk/discord.h"
#include "ps2.hpp"

#include "appdata/assets.hpp"
#include "game/state.hpp"
#include "moc_macros.hpp"

namespace PresenceApp {
PresenceFactory::PresenceFactory(QObject* parent)
    : QObject{ parent }
    , is_idle_{ true }
{
    // Emit initial idle activity
    setActivityIdle();
    emit activityChanged(getPresenceAsActivity());
}

discord::Activity PresenceFactory::getPresenceAsActivity() {
    discord::Activity activity{};
    if (is_idle_) {
        activity = buildIdleActivity();
    }
    else {
        activity = buildGameActivity(state_);
    }
    return activity;
}

void PresenceFactory::setActivityIdle() {
    if (!is_idle_) {
        is_idle_ = true;
        emit activityChanged(getPresenceAsActivity());
    }
}

void PresenceFactory::setActivityFromGameState(const GameState& state) {
    if (state_ != state || is_idle_) {
        state_ = state;
        is_idle_ = false;
        emit activityChanged(getPresenceAsActivity());
    }
}

discord::Activity PresenceFactory::buildIdleActivity() {
    discord::Activity activity{};
    activity.SetState("Idling");
    activity.SetType(discord::ActivityType::Playing);
    auto& assets = activity.GetAssets();
    std::string temp;
    assets::imageKeyFromZone(ps2::Zone::Sanctuary, &temp);
    assets.SetLargeImage(temp.c_str());
    // TODO: Tidy up asset/text pair generation
    assets.SetLargeText("Sanctuary");
    return activity;
}

discord::Activity PresenceFactory::buildGameActivity(const GameState& state) {
    discord::Activity activity{};
    std::string temp;
    // Details
    ps2::faction_to_display_name(state.faction_, &temp);
    QString details;
    if (state.faction_ == state.team_) {
        details = QString::fromStdString(temp);
    }
    else {
        ps2::faction_to_display_name(state.team_, &temp);
        details = "Freelancing for " + QString::fromStdString(temp);
    }
    activity.SetDetails(details.toStdString().c_str());
    // State
    ps2::server_to_display_name(state.server_, &temp);
    activity.SetState(temp.c_str());
    auto& assets = activity.GetAssets();
    // Large image
    assets::imageKeyFromZone(state.zone_, &temp);
    assets.SetLargeImage(temp.c_str());
    ps2::zone_to_display_name(state.zone_, &temp);
    assets.SetLargeText(temp.c_str());
    // Small image
    std::string key, text;
    if (state.vehicle_ != ps2::Vehicle::None) {
        assets::imageKeyFromVehicle(state.vehicle_, &temp);
        key = temp;
        ps2::vehicle_to_display_name(state.vehicle_, &temp);
        text = temp;
    }
    else {
        assets::imageKeyFromClass(state.class_, &temp);
        key = temp;
        ps2::class_to_display_name(state.class_, &temp);
        text = temp;
    }
    assets.SetSmallImage(key.c_str());
    assets.SetSmallText(text.c_str());
    return activity;
}

} // namespace PresenceApp

PUSH_MOC_WARNINGS_FILTER;
#include "moc_factory.cpp"
POP_MOC_WARNINGS_FILTER;
