// Copyright 2022 Leonhard S.

#include "persistence.hpp"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QStandardPaths>
#include <QtCore/QString>

#include "arx.hpp"
#include "ps2.hpp"

#include "game/character-info.hpp"

namespace {

QJsonObject characterToJson(const PresenceApp::CharacterData& character) {
    QJsonObject json;
    json["id"] = QString::number(character.id_);
    json["name"] = character.name_;
    json["faction"] = ps2::faction_to_faction_id(character.faction_);
    json["last_profile"] = static_cast<quint16>(ps2::class_to_profile_id(
        character.class_, character.faction_));
    json["world"] = ps2::server_to_world_id(character.server_);
    return json;
}

PresenceApp::CharacterData characterFromJson(const QJsonObject& json) {
    PresenceApp::CharacterData char_;
    char_.id_ = json["id"].toString().toULongLong();
    char_.name_ = json["name"].toString();
    auto faction_ok = ps2::faction_from_faction_id(
        static_cast<arx::faction_id_t>(json["faction"].toInt()), &char_.faction_);
    auto class_ok = ps2::class_from_profile_id(
        static_cast<arx::profile_id_t>(json["last_profile"].toInt()), &char_.class_);
    auto server_ok = ps2::server_from_world_id(
        static_cast<arx::world_id_t>(json["world"].toInt()), &char_.server_);
    if (!faction_ok || !class_ok || !server_ok) {
        qWarning() << "Failed to load character data from JSON";
        // TODO: Propagate error state
    }
    return char_;
}

static bool keyIsValid(const QString& key) {
    return (key == "characters" ||
        key == "auto_track" ||
        key == "start_with_os" ||
        key == "minimise_to_tray" ||
        key == "presence_enabled");
}

QVariantMap loadConfigVersion1_0(const QJsonObject& json) {
    QVariantMap config;

    // Iterate over json keys
    for (auto it = json.constBegin(); it != json.constEnd(); ++it) {
        // Ignore invalid keys
        if (!keyIsValid(it.key())) {
            continue;
        }
        // Handle the characters list separately
        if (it.key() == "characters") {
            // Load all characters
            QVariantList characters_list;
            for (auto&& char_json : it.value().toArray()) {
                auto char_data = characterFromJson(char_json.toObject());
                characters_list.append(QVariant::fromValue(char_data));
            }
            config["characters"] = characters_list;
        }
        else {
            // Copy other keys without change
            config[it.key()] = it.value().toVariant();
        }
    }
    return config;
}

QString getConfigFilePath() {
    QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    // Create config directory if it does not exist
    QDir().mkpath(dir);
    // Append filename
    return dir + QDir::separator() + "settings.json";
}

} // namespace

namespace PresenceApp {

void AppConfigManager::save(const QVariantMap& config) {
    QJsonObject json_object;

    for (auto it = config.cbegin(); it != config.cend(); ++it) {
        // Ignore invalid keys
        if (!keyIsValid(it.key())) {
            continue;
        }
        // Handle the characters list separately
        if (it.key() == "characters") {
            // Load all characters into the JSON list
            QJsonArray characters;
            for (const auto& character : it.value().toList()) {
                characters.append(
                    characterToJson(character.value<CharacterData>()));
            }
            json_object["characters"] = characters;
        }
        else {
            // Copy other keys without change
            json_object.insert(
                it.key(), QJsonValue::fromVariant(it.value()));
        }
    }

    // Add config file version identifier
    json_object["version"] = "1.0";

    // Write config file to disk
    QJsonDocument json_document(json_object);
    QString json_string = json_document.toJson(QJsonDocument::Indented);
    QFile file(getConfigFilePath());
    file.open(QIODevice::WriteOnly);
    file.write(json_string.toUtf8());
    file.close();
}

QVariantMap AppConfigManager::load() {
    QVariantMap config;
    // See if the config file exists
    QString path = getConfigFilePath();
    if (QFile::exists(path)) {
        // Load config file from disk
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        QJsonDocument json_document = QJsonDocument::fromJson(file.readAll());
        file.close();
        // Check version
        if (json_document.object().contains("version") &&
            json_document.object()["version"].toString() == "1.0") {
            config = loadConfigVersion1_0(json_document.object());
        }
        else {
            qWarning() << "Unsupported config file version, initialising with defaults";
            config = defaults();
        }
    }
    else {
        // If not, create it with default values
        config = defaults();
        save(config);
    }
    return config;
}

QVariantMap AppConfigManager::defaults() {
    QVariantMap config;
    config["characters"] = QVariantList();
    config["auto_track"] = false;
    config["start_with_os"] = false;
    config["minimise_to_tray"] = true;
    config["presence_enabled"] = false;
    return config;
}

} // namespace PresenceApp
