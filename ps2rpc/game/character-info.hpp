// Copyright 2022 Leonhard S.

#ifndef PS2RPC_GAME_CHARACTER_INFO_HPP
#define PS2RPC_GAME_CHARACTER_INFO_HPP

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <QtCore/QString>
#include <QtNetwork/QNetworkAccessManager>

#include "ps2.hpp"

namespace ps2rpc
{

    class CharacterInfo : public QObject
    {
        Q_OBJECT

    public:
        CharacterInfo(QObject *parent = nullptr);
        CharacterInfo(ps2::CharacterId id, QObject *parent = nullptr);
        CharacterInfo(ps2::CharacterId id,
                      const QString &name,
                      ps2::Faction faction,
                      ps2::Class class_,
                      ps2::Server world,
                      QObject *parent = nullptr);

        ps2::CharacterId getId() const;
        QString getName() const;
        ps2::Faction getFaction() const;
        ps2::Class getClass() const;
        ps2::Server getWorld() const;

    Q_SIGNALS:
        void infoChanged();

    private:
        void updateFieldsIfChanged(ps2::CharacterId id,
                                   const QString &name,
                                   ps2::Faction faction,
                                   ps2::Class class_,
                                   ps2::Server world);

        ps2::CharacterId id_;
        QString name_;
        ps2::Faction faction_;
        ps2::Class class_;
        ps2::Server world_;

        QScopedPointer<QNetworkAccessManager> manager_;
    };

} // namespace ps2rpc

#endif // PS2RPC_GAME_CHARACTER_INFO_HPP
