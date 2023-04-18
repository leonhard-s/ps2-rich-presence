// Copyright 2022 Leonhard S.

#pragma once

#include <QtCore/QDateTime>
#include <QtCore/QObject>
#include <QtCore/QString>

namespace PresenceApp {

/**
 * Returns a string describing the time since the given timestamp.
 *
 * This is limited to between "just now" and "over an hour ago".
 */
QString getTimeAgo(const QDateTime& timestamp) {
    // Get time diference in seconds
    const auto time_ago = timestamp.secsTo(QDateTime::currentDateTime());
    // Return time ago in a human readable format
    if (time_ago < 5) {
        return QCoreApplication::translate("TimeAgo", "just now");
    }
    else if (time_ago < 60) {
        return QCoreApplication::translate("TimeAgo", "%1 seconds ago").arg(time_ago);
    }
    else if (time_ago < 3600) {
        if (time_ago / 60 == 1) {
            return QCoreApplication::translate("TimeAgo", "1 minute ago");
        }
        else {
            return QCoreApplication::translate("TimeAgo", "%1 minutes ago")
                .arg(time_ago / 60);
        }
    }
    else if (timestamp == QDateTime::fromSecsSinceEpoch(0)) {
        return QCoreApplication::translate("TimeAgo", "never");
    }
    else {
        return QCoreApplication::translate("TimeAgo", "over an hour ago");
    }
}

} // namespace PresenceApp
