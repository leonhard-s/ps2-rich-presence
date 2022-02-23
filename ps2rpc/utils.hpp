// Copyright 2022 Leonhard S.

#ifndef PS2RPC_UTILS_HPP
#define PS2RPC_UTILS_HPP

#include <QtCore/QJsonObject>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkReply>

#include "arx.hpp"

namespace ps2rpc
{

    QUrl qUrlFromArxQuery(const arx::Query &query);

    QJsonObject getJsonPayload(QNetworkReply &reply);

} // namespace ps2rpc

#endif // PS2RPC_UTILS_HPP
