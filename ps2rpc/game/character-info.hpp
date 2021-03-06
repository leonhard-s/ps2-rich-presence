// Copyright 2022 Leonhard S.

#ifndef PS2RPC_GAME_CHARACTER_INFO_HPP
#define PS2RPC_GAME_CHARACTER_INFO_HPP

#include <QtCore/QDebug>
#include <QtCore/QMetaType>
#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <QtCore/QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

#include "arx.hpp"
#include "ps2.hpp"

namespace ps2rpc
{

    struct CharacterData
    {
        CharacterData();
        CharacterData(arx::character_id_t id,
                      const QString &name,
                      ps2::Faction faction,
                      ps2::Class class_,
                      ps2::Server server);
        ~CharacterData() = default;
        CharacterData &operator=(const CharacterData &) = default;

        bool operator==(const CharacterData &other) const;
        bool operator!=(const CharacterData &other) const;

        arx::character_id_t id;
        QString name;
        ps2::Faction faction;
        ps2::Class class_;
        ps2::Server server;
    };

    QDebug operator<<(QDebug dbg, const CharacterData &info);

    class CharacterInfo : public QObject
    {
        Q_OBJECT

    public:
        CharacterInfo(QObject *parent = nullptr);
        CharacterInfo(arx::character_id_t id, QObject *parent = nullptr);
        CharacterInfo(arx::character_id_t id,
                      const QString &name,
                      ps2::Faction faction,
                      ps2::Class class_,
                      ps2::Server server,
                      QObject *parent = nullptr);

        arx::character_id_t getId() const;
        QString getName() const;
        ps2::Faction getFaction() const;
        ps2::Class getClass() const;
        ps2::Server getServer() const;

        void handleCharacterInfoPayload(const arx::json_t &payload);

    Q_SIGNALS:
        void infoChanged();

    public Q_SLOTS:
        void populate();

    private Q_SLOTS:
        void onCharacterInfoRequestFinished();

    private:
        QNetworkRequest getCharacterInfoRequest();
        void updateFieldsIfChanged(arx::character_id_t id,
                                   const QString &name,
                                   ps2::Faction faction,
                                   ps2::Class class_,
                                   ps2::Server server);

        CharacterData info_;
        QScopedPointer<QNetworkAccessManager> manager_;
    };

} // namespace ps2rpc

Q_DECLARE_METATYPE(ps2rpc::CharacterData)

#endif // PS2RPC_GAME_CHARACTER_INFO_HPP
