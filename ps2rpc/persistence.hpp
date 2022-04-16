// Copyright 2022 Leonhard S.

#ifndef PS2RPC_PERSISTENCE_HPP
#define PS2RPC_PERSISTENCE_HPP

#include <QtCore/QVariantMap>

#include "game/character-info.hpp"

namespace ps2rpc
{

    class AppConfigManager
    {
    public:
        AppConfigManager() = default;

        static void save(const QVariantMap &config);
        static QVariantMap load();
        static QVariantMap defaults();
    };

} // namespace ps2rpc

#endif // PS2RPC_PERSISTENCE_HPP
