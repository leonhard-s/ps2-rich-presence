// Copyright 2022 Leonhard S.

#ifndef PS2RPC_CORE_HPP
#define PS2RPC_CORE_HPP

#include <QtCore/QJsonObject>
#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <QtCore/QString>

#include "game/character-info.hpp"
#include "presence/factory.hpp"
#include "presence/handler.hpp"
#include "tracker.hpp"

namespace ps2rpc
{

    class RichPresenceApp : public QObject
    {
        Q_OBJECT

    public:
        explicit RichPresenceApp(QObject *parent = nullptr);

        bool hasCharacter() const;
        CharacterData getCharacter() const;

    public Q_SLOTS:
        void setCharacter(const CharacterData &character);

    private Q_SLOTS:
        void onGameStateChanged(const GameState &state);
        void onEssPayloadReceived(const QString &event_name,
                                  const QJsonObject &payload);

    private:
        QScopedPointer<ActivityTracker> tracker_;
        QScopedPointer<PresenceHandler> discord_;
        QScopedPointer<PresenceFactory> factory_;
    };

} // namespace ps2rpc

#endif // PS2RPC_CORE_HPP
