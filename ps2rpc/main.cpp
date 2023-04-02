// Copyright 2022 Leonhard S.

#include <QApplication>
#include <QCoreApplication>

#include "gui/main-window.hpp"
#include "config.hpp"

using namespace ps2rpc;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Leonhard S.");
    QCoreApplication::setApplicationName("PS2 Rich Presence");
    QCoreApplication::setApplicationVersion(PS2RPC_VERSION);

    MainWindow main_window;
    main_window.show();

    return app.exec();
}
