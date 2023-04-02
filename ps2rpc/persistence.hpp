// Copyright 2022 Leonhard S.

#pragma once

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
