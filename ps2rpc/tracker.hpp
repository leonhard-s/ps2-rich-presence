// Copyright 2022 Leonhard S.

#ifndef PS2RPC_TRACKER_HPP
#define PS2RPC_TRACKER_HPP

#include <QtCore/QJsonObject>
#include <QtCore/QObject>
#include <QtCore/QScopedPOinter>
#include <QtCore/QString>

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
        explicit ActivityTracker(ps2::CharacterId character_id);

    Q_SIGNALS:
        void ready();
        void stateChanged(GameState state);

    private Q_SLOTS:
        void onPayloadReceived(const QString &event_name, const QJsonObject &payload);
        void onCharacterInfoUpdated();

    private:
        arx::Subscription generateSubscription() const;

        ps2::CharacterId character_id_;
        GameStateFactory state_factory_;
        GameState current_state_;
        QScopedPointer<CharacterInfo> char_info_;
        QScopedPointer<arx::EssClient> ess_client_;
    };

} // namespace ps2rpc

#endif // PS2RPC_TRACKER_HPP
