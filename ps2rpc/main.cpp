// Copyright 2022 Leonhard S.

#include <QApplication>

#include "config.hpp"

using namespace ps2rpc;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Leonhard S.");
    QCoreApplication::setApplicationName("PS2 Rich Presence");
    QCoreApplication::setApplicationVersion(PS2RPC_VERSION);
    return app.exec();
}
