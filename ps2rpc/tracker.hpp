// Copyright 2022 Leonhard S.

#ifndef PS2RPC_TRACKER_HPP
#define PS2RPC_TRACKER_HPP

#include <QtCore/QJsonObject>
#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <QtCore/QString>

#include "arx.hpp"
#include "arx/ess.hpp"
#include "ps2.hpp"

#include "game/character-info.hpp"
#include "game/state.hpp"

namespace ps2rpc
{

    class ActivityTracker : public QObject
    {
        Q_OBJECT

    public:
        ActivityTracker(const CharacterData &character,
                        QObject *parent = nullptr);

        CharacterData getCharacter() const;

    Q_SIGNALS:
        void ready();
        void stateChanged(GameState state);
        void payloadReceived(const QString &event_name,
                             const QJsonObject &payload);

    private Q_SLOTS:
        void onPayloadReceived(const QString &event_name,
                               const arx::json_object &payload);

    private:
        arx::Subscription generateSubscription() const;

        CharacterData character_;
        GameStateFactory state_factory_;
        GameState current_state_;
        QScopedPointer<arx::EssClient> ess_client_;
    };

} // namespace ps2rpc

#endif // PS2RPC_TRACKER_HPP
