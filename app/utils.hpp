// Copyright 2022 Leonhard S.

#pragma once

#include <string>

#include <QtCore/QUrl>

#include "arx.hpp"
#include "ps2.hpp"

class QJsonDocument;

namespace PresenceApp {

QString qStringFromArxQuery(const arx::Query& query);

arx::json_t qtJsonToArxJson(const QJsonDocument& data);

arx::character_id_t characterIdFromJson(const arx::json_t& object);

std::string characterNameFromJson(const arx::json_t& object);

ps2::Faction factionFromJson(const arx::json_t& object);

ps2::Class classFromJsonLoadout(const arx::json_t& object);

ps2::Class classFromJsonProfile(const arx::json_t& object);

ps2::Server serverFromJson(const arx::json_t& object);

} // namespace PresenceApp
