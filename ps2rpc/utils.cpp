// Copyright 2022 Leonhard S.

#include "utils.hpp"

#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>

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

} // namespace ps2rpc
