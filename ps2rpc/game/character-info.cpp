// Copyright 2022 Leonhard S.

#include "game/character-info.hpp"

#include <QtCore/QObject>
#include <QtCore/QString>

#include "arx.hpp"
#include "ps2.hpp"

namespace ps2rpc
{

    CharacterInfo::CharacterInfo(QObject *parent)
        : QObject(parent), id_{0}, name_{}, faction_{ps2::Faction::NS},
          class_{ps2::Class::LightAssault}, world_{ps2::Server::Connery} {}

    CharacterInfo::CharacterInfo(ps2::CharacterId id, QObject *parent)
        : CharacterInfo(parent)
    {
        id_ = id;
    }

    CharacterInfo::CharacterInfo(ps2::CharacterId id,
                                 const QString &name,
                                 ps2::Faction faction,
                                 ps2::Class class_,
                                 ps2::Server world,
                                 QObject *parent)
        : CharacterInfo(id, parent)
    {
        name_ = name;
        faction_ = faction;
        this->class_ = class_;
        world_ = world;
    }

    ps2::CharacterId CharacterInfo::getId() const
    {
        return id_;
    }

    QString CharacterInfo::getName() const
    {
        return name_;
    }

    ps2::Faction CharacterInfo::getFaction() const
    {
        return faction_;
    }

    ps2::Class CharacterInfo::getClass() const
    {
        return class_;
    }

    ps2::Server CharacterInfo::getWorld() const
    {
        return world_;
    }

    void CharacterInfo::updateFieldsIfChanged(ps2::CharacterId id,
                                              const QString &name,
                                              ps2::Faction faction,
                                              ps2::Class class_,
                                              ps2::Server world)
    {
        bool changed = false;
        if (id != id_)
        {
            id_ = id;
            changed = true;
        }
        if (name != name_)
        {
            name_ = name;
            changed = true;
        }
        if (faction != faction_)
        {
            faction_ = faction;
            changed = true;
        }
        if (class_ != this->class_)
        {
            this->class_ = class_;
            changed = true;
        }
        if (world != world_)
        {
            world_ = world;
            changed = true;
        }
        // Emit signal if any fields changed
        if (changed)
        {
            emit infoChanged();
        }
    }

} // namespace ps2rpc
