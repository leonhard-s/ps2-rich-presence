// Copyright 2022 Leonhard S.

#ifndef PS2RPC_UTILS_HPP
#define PS2RPC_UTILS_HPP

#include <string>

#include <QtCore/QUrl>
#include <QtNetwork/QNetworkReply>

#include "arx.hpp"
#include "ps2.hpp"

namespace ps2rpc
{

    QUrl qUrlFromArxQuery(const arx::Query &query);

    arx::json_t getJsonPayload(QNetworkReply &reply);

    arx::character_id_t characterIdFromJson(const arx::json_t &object);

    std::string characterNameFromJson(const arx::json_t &object);

    ps2::Faction factionFromJson(const arx::json_t &object);

    ps2::Class classFromJsonLoadout(const arx::json_t &object);

    ps2::Class classFromJsonProfile(const arx::json_t &object);

    ps2::Server serverFromJson(const arx::json_t &object);

} // namespace ps2rpc

#endif // PS2RPC_UTILS_HPP
