// Copyright 2022 Leonhard S.

#include "utils.hpp"

#include <string>

#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>
#include <QtNetwork/QNetworkReply>

#include "arx.hpp"
#include "ps2.hpp"

namespace {

arx::json_string_t stringViaJsonKey(const arx::json_t& object,
    const arx::json_string_t& key) {
    auto it = object.find(key);
    if (it == object.end()) {
        qWarning() << "JSON key not found:" << key.c_str() << "\n"
            << object.dump().c_str();
        return "";
    }
    if (!it->is_string()) {
        qWarning() << "JSON key is not a string:" << key.c_str() << "\n"
            << object.dump().c_str();
        return "";
    }
    return it->get<arx::json_string_t>();
}

template <typename T>
T quotedIntegerViaJsonKey(const arx::json_t& object,
    const arx::json_string_t& key) {
    auto val_str = stringViaJsonKey(object, key);
    if (val_str == "") {
        return 0;
    }
    return static_cast<T>(std::strtoull(val_str.c_str(), nullptr, 10));
}

} // namespace

namespace ps2rpc {

QUrl qUrlFromArxQuery(const arx::Query& query) {
    QUrl url;
    url.setScheme(QString::fromStdString(query.getScheme()));
    url.setHost(QString::fromStdString(query.getHost()));
    url.setPath(QString::fromStdString(query.getPath()));
    QUrlQuery q;
    for (const auto& item : query.getQuery()) {
        q.addQueryItem(QString::fromStdString(item.first),
            QString::fromStdString(item.second));
    }
    url.setQuery(q);
    return url;
}

arx::json_t getJsonPayload(QNetworkReply& reply) {
    return arx::json_t::parse(reply.readAll().toStdString());
}

arx::character_id_t characterIdFromJson(const arx::json_t& object) {
    auto id_str = stringViaJsonKey(object, "character_id");
    if (id_str.empty()) {
        qWarning() << "Invalid character ID" << id_str.c_str();
        return 0;
    }
    return std::strtoull(id_str.c_str(), nullptr, 10);
}

std::string characterNameFromJson(const arx::json_t& object) {
    auto it = object.find("name");
    if (it == object.end()) {
        qWarning() << "JSON key not found: name\n"
            << object.dump().c_str();
        return "N/A";
    }
    if (!it->is_object()) {
        qWarning() << "JSON key is not an object: name\n"
            << object.dump().c_str();
        return "N/A";
    }
    return stringViaJsonKey(*it, "first");
}

ps2::Faction factionFromJson(const arx::json_t& object) {
    auto id = quotedIntegerViaJsonKey<arx::faction_id_t>(object, "faction_id");
    ps2::Faction faction = ps2::Faction::NS;
    if (ps2::faction_from_faction_id(id, faction)) {
        qWarning() << "Invalid faction_id" << id;
    }
    return faction;
}

ps2::Class classFromJsonLoadout(const arx::json_t& payload) {
    auto id = quotedIntegerViaJsonKey<arx::loadout_id_t>(payload, "loadout_id");
    ps2::Class class_ = ps2::Class::LightAssault;
    if (ps2::class_from_loadout_id(id, class_)) {
        qWarning() << "Invalid loadout_id" << id;
    }
    return class_;
}

ps2::Class classFromJsonProfile(const arx::json_t& payload) {
    auto id = quotedIntegerViaJsonKey<arx::profile_id_t>(payload, "profile_id");
    ps2::Class class_ = ps2::Class::LightAssault;
    if (ps2::class_from_profile_id(id, class_)) {
        qWarning() << "Invalid profile_id" << id;
    }
    return class_;
}

ps2::Server serverFromJson(const arx::json_t& object) {
    auto id = quotedIntegerViaJsonKey<arx::world_id_t>(object, "world_id");
    ps2::Server server = ps2::Server::Connery;
    if (ps2::server_from_world_id(id, server)) {
        qWarning() << "Invalid world" << id;
    }
    return server;
}

} // namespace ps2rpc
