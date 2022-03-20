// Copyright 2022 Leonhard S:

#ifndef ARX_ESS_EVENTS_HPP
#define ARX_ESS_EVENTS_HPP

#include "arx/types.hpp"

namespace arx
{

    enum class Event
    {
        Unknown, // Fallback value, not used by ESS

        AchievementEarned,
        BattleRankUp,
        ContinentLock,
        ContinentUnlock,
        Death,
        FacilityControl,
        GainExperience,
        ItemAdded,
        MetagameEvent,
        PlayerFacilityCapture,
        PlayerFacilityDefend,
        PlayerLogin,
        PlayerLogout,
        SkillAdded,
        VehicleDestroy,
    };

    Event eventFromEventName(const string_t &event_name);

    string_t eventToEventName(const Event &event);

    bool isEventCharacterCentric(const Event &event);

    bool isEventWorldCentric(const Event &event);

} // namespace arx

#endif // ARX_ESS_EVENTS_HPP
