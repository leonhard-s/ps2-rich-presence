// Copyright 2022 Leonhard S.

#include "utils.hpp"

#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>
#include <QtNetwork/QNetworkReply>

#include "arx.hpp"

namespace ps2rpc
{

    QUrl qUrlFromArxQuery(const arx::Query &query)
    {
        QUrl url;
        url.setScheme(QString::fromStdString(query.getScheme()));
        url.setHost(QString::fromStdString(query.getHost()));
        url.setPath(QString::fromStdString(query.getPath()));
        QUrlQuery q;
        for (const auto &item : query.getQuery())
        {
            q.addQueryItem(QString::fromStdString(item.first),
                           QString::fromStdString(item.second));
        }
        url.setQuery(q);
        return url;
    }

    QJsonObject getJsonPayload(QNetworkReply &reply)
    {
        // Seek to teh beginning of the data in case it was already read before
        reply.seek(0);
        auto data = static_cast<QString>(reply.readAll());
        auto doc = QJsonDocument::fromJson(data.toUtf8());
        return doc.object();
    }

} // namespace ps2rpc
