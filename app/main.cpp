// Copyright 2022 Leonhard S.

#include <QApplication>
#include <QCoreApplication>

#include "gui/main-window.hpp"
#include "config.hpp"


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Leonhard S.");
    QCoreApplication::setApplicationName("PS2 Rich Presence");
    QCoreApplication::setApplicationVersion(PRESENCE_APP_VERSION);

    PresenceApp::MainWindow main_window;
    main_window.show();

    return app.exec();
}
