// Copyright 2022 Leonhard S.

#ifndef PS2RPC_GUI_TIMEAGO_HPP
#define PS2RPC_GUI_TIMEAGO_HPP

#include <QtCore/QDateTime>
#include <QtCore/QObject>
#include <QtCore/QString>

namespace ps2rpc
{

    /**
     * Returns a string describing the time since the given timestamp.
     *
     * This is limited to between "just now" and "over an hour ago".
     */
    QString getTimeAgo(const QDateTime &timestamp)
    {
        // Get time diference in seconds
        const auto time_ago = timestamp.secsTo(QDateTime::currentDateTime());
        // Return time ago in a human readable format
        if (time_ago < 5)
        {
            return QCoreApplication::translate("TimeAgo", "just now");
        }
        else if (time_ago < 60)
        {
            return QCoreApplication::translate("TimeAgo", "%1 seconds ago").arg(time_ago);
        }
        else if (time_ago < 3600)
        {
            return QCoreApplication::translate("TimeAgo", "%1 minute(s) ago").arg(time_ago / 60);
        }
        else if (timestamp == QDateTime::fromSecsSinceEpoch(0))
        {
            return QCoreApplication::translate("TimeAgo", "never");
        }
        else
        {
            return QCoreApplication::translate("TimeAgo", "over an hour ago");
        }
    }

} // namespace ps2rpc

#endif // PS2RPC_GUI_TIMEAGO_HPP
