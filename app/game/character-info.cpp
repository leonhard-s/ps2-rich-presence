// Copyright 2022 Leonhard S.

#include "character-info.hpp"

#include <string>
#include <utility>

#include <QDebug>
#include <QObject>
#include <QString>
#include <api/rest_client.h>
#include <arx.hpp>
#include <ps2.hpp>
#include <adopt_pointer.h>
#include <moc_macros.h>

#include "appdata/service-id.hpp"
#include "utils.hpp"

namespace PresenceApp {

CharacterData::CharacterData()
    : id_{ 0 }
    , name_{ "" }
    , faction_{ ps2::Faction::NS }
    , class_{ ps2::Class::LightAssault }
    , server_{ ps2::Server::Connery } {}

CharacterData::CharacterData(
    arx::character_id_t id,
    QString name,
    ps2::Faction faction,
    ps2::Class cls,
    ps2::Server server
)
    : id_{ id }
    , name_{ std::move(name) }
    , faction_{ faction }
    , class_{ cls }
    , server_{ server } {}

bool CharacterData::operator==(const CharacterData& other) const {
    return id_ == other.id_ &&
        name_ == other.name_ &&
        faction_ == other.faction_ &&
        class_ == other.class_ &&
        server_ == other.server_;
}

bool CharacterData::operator!=(const CharacterData& other) const {
    return !(*this == other);
}

QDebug operator<<(QDebug dbg, const CharacterData& info) {
    std::string tag;
    ps2::faction_to_tag(info.faction_, &tag);
    return dbg << info.name_ << "[" << QString::fromStdString(tag) << "]";
}

CharacterInfo::CharacterInfo(QObject* parent)
    : QObject(parent)
{}

CharacterInfo::CharacterInfo(arx::character_id_t id, QObject* parent)
    : CharacterInfo(parent)
{
    info_.id_ = id;
}

CharacterInfo::CharacterInfo(arx::character_id_t id,
    QString name,
    ps2::Faction faction,
    ps2::Class cls,
    ps2::Server server,
    QObject* parent
)
    : CharacterInfo(id, parent)
{
    info_.name_ = std::move(name);
    info_.faction_ = faction;
    info_.class_ = cls;
    info_.server_ = server;
}

arx::character_id_t CharacterInfo::getId() const {
    return info_.id_;
}

QString CharacterInfo::getName() const {
    return info_.name_;
}

ps2::Faction CharacterInfo::getFaction() const {
    return info_.faction_;
}

ps2::Class CharacterInfo::getClass() const {
    return info_.class_;
}

ps2::Server CharacterInfo::getServer() const {
    return info_.server_;
}

void CharacterInfo::populate() {
    // Only look up sensible character IDs
    if (info_.id_ <= 0) {
        qWarning() << "Call to CharacterInfo::populate() ignored due to "
            "invalid character ID:"
            << info_.id_;
        return;
    }
    // Generate request
    auto request = getCharacterInfoRequest();
    // Reply object will be deleted by reply handler
    auto* const client = adopt_pointer(new PresenceLib::AsyncRestClient(this));
    QObject::connect(client, &PresenceLib::AsyncRestClient::requestFinished,
        this, &CharacterInfo::onCharacterInfoRequestFinished);
    QObject::connect(client, &PresenceLib::AsyncRestClient::requestFinished,
        client, &QObject::deleteLater);
    QObject::connect(client, &PresenceLib::AsyncRestClient::requestFailed,
        client, &QObject::deleteLater);
}

void CharacterInfo::onCharacterInfoRequestFinished(const QJsonDocument& response) {
    auto payload = qtJsonToArxJson(response);
    handleCharacterInfoPayload(payload);
}

QString CharacterInfo::getCharacterInfoRequest() const {
    arx::Query query("character", SERVICE_ID);
    query.addTerm(
        arx::SearchTerm("character_id", std::to_string(info_.id_)));
    query.setShow({ "character_id", "name.first", "faction_id", "profile_id" });
    auto join = arx::JoinData("characters_world");
    join.show_.emplace_back("world_id");
    query.addJoin(join);
    // Generate URL
    return qStringFromArxQuery(query);
}

void CharacterInfo::handleCharacterInfoPayload(const arx::json_t& payload) {
    if (arx::validatePayload("character", payload) != 0) {
        qWarning() << "CharacterInfo::handleCharacterInfoPayload(): "
            "Invalid JSON payload";
        return;
    }
    // Get character object
    auto data = arx::payloadResultAsObject("character", payload);
    // Get new fields from payload
    auto new_id = characterIdFromJson(data);
    auto new_name = QString::fromStdString(characterNameFromJson(data));
    auto new_faction = factionFromJson(data);
    auto new_class = classFromJsonProfile(data);
    // Resolve characters_world join
    auto new_server = ps2::Server::Connery;
    auto world_data = data.find("world");
    if (world_data == data.end()) {
        qWarning() << "CharacterInfo::handleCharacterInfoPayload():"
            << "No world join data in payload";
    }
    else if (!world_data->is_object()) {
        qWarning() << "CharacterInfo::handleCharacterInfoPayload():"
            << "World join data is not an object";
    }
    else {
        new_server = serverFromJson(*world_data);
    }
    // Update fields
    updateFieldsIfChanged(
        new_id, new_name, new_faction, new_class, new_server);
}

void CharacterInfo::updateFieldsIfChanged(
    arx::character_id_t id,
    QString name,
    ps2::Faction faction,
    ps2::Class cls,
    ps2::Server server
) {
    bool changed = false;
    if (id != info_.id_) {
        info_.id_ = id;
        changed = true;
    }
    if (name != info_.name_) {
        info_.name_ = std::move(name);
        changed = true;
    }
    if (faction != info_.faction_) {
        info_.faction_ = faction;
        changed = true;
    }
    if (cls != info_.class_) {
        info_.class_ = cls;
        changed = true;
    }
    if (server != info_.server_) {
        info_.server_ = server;
        changed = true;
    }
    // Emit signal if any fields changed
    if (changed) {
        emit infoChanged();
    }
}

} // namespace PresenceApp

PUSH_MOC_WARNINGS_FILTER;
#include "moc_character-info.cpp"
POP_MOC_WARNINGS_FILTER;
