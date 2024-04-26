// Copyright 2022 Leonhard S.

#pragma once

#include <memory>

#include <QDialog>
#include <QListWidget>
#include <QObject>
#include <QPushButton>
#include <QString>
#include <arx.hpp>

#include "game/character-info.hpp"

class QJsonDocument;

namespace PresenceApp {

class CharacterManager : public QDialog {
    Q_OBJECT

public:
    explicit CharacterManager(QWidget* parent = nullptr);
    CharacterManager(const CharacterManager& other) = delete;
    CharacterManager(CharacterManager&& other) noexcept = delete;

    CharacterManager& operator=(const CharacterManager& other) = delete;
    CharacterManager& operator=(CharacterManager&& other) noexcept = delete;

    void addCharacter(const CharacterData& character);

Q_SIGNALS:
    void characterAdded(int index, const CharacterData& name);
    void characterRemoved(int index, const CharacterData& name);

private Q_SLOTS:
    void onAddButtonClicked();
    void onRemoveButtonClicked();
    void onCharacterSelected();
    void onCharacterInfoReceived(const QJsonDocument& response);

private:
    static QString getCharacterInfoUrl(const QString& character);
    static CharacterData parseCharacterPayload(const arx::json_t& payload);
    QDialog* createCharacterNameInputDialog();
    void setupUi();

    QListWidget* list_;
    QPushButton* button_add_;
    QPushButton* button_remove_;
    QPushButton* button_close_;
    bool is_loading_;
};

} // namespace PresenceApp
