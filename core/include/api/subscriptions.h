// Copyright 2024 Leonhard S.
#pragma once

#include <cstdint>

class QJsonObject;

namespace PresenceLib {

QJsonObject getDeathSubscription(int64_t character_id);

QJsonObject getExperienceSubscription(int64_t character_id);

} // namespace PresenceLib
