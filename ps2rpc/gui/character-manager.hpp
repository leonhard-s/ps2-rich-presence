// Copyright 2022 Leonhard S.

#ifndef PS2RPC_GUI_CHARACTER_MANAGER_HPP
#define PS2RPC_GUI_CHARACTER_MANAGER_HPP

#include <QtCore/QJsonObject>
#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtWidgets/QDialog>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>

#include "arx.hpp"

#include "game/character-info.hpp"

namespace ps2rpc
{

    class CharacterManager : public QDialog
    {
        Q_OBJECT

    public:
        explicit CharacterManager(QWidget *parent = nullptr);

        void addCharacter(const CharacterData &character);

    Q_SIGNALS:
        void characterAdded(int index, const CharacterData &name);
        void characterRemoved(int index, const CharacterData &name);

    private Q_SLOTS:
        void onAddButtonClicked();
        void onRemoveButtonClicked();
        void onCharacterSelected();
        void onCharacterInfoReceived();

    private:
        QUrl getCharacterInfoUrl(const QString &character) const;
        CharacterData parseCharacterPayload(const arx::json_object &payload);
        QDialog *createCharacterNameInputDialog();
        void setupUi();

        QScopedPointer<QNetworkAccessManager> manager_;

        QListWidget *list_;
        QPushButton *button_add_;
        QPushButton *button_remove_;
        QPushButton *button_close_;
    };

} // namespace ps2pc

#endif // PS2RPC_GUI_CHARACTER_MANAGER_HPP
