// Copyright 2022 Leonhard S.

#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <QtWidgets/QApplication>

#include "discord.h"
#include "arx/ess.hpp"

#include "game/state.hpp"
#include "presence/handler.hpp"
#include "presence/factory.hpp"
#include "tracker.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Create presence helpers
    ps2rpc::PresenceHandler presence_handler;
    ps2rpc::PresenceFactory factory;
    factory.setActivityIdle();

    // Create activity tracker
    // TODO: Allow input of tracked character name/ID
    ps2::CharacterId character_id = 5428072203494645969;
    ps2rpc::ActivityTracker tracker{character_id};

    // Hook up game state tracking to presence handler
    QObject::connect(&tracker, &ps2rpc::ActivityTracker::stateChanged, &factory, &ps2rpc::PresenceFactory::setActivityFromGameState);
    QObject::connect(&factory, &ps2rpc::PresenceFactory::activityChanged, &presence_handler, &ps2rpc::PresenceHandler::setActivity);

    return a.exec();
}
