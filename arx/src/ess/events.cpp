// Copyright 2022 Leonhard S.

#include "arx/ess/events.hpp"

#include "arx/types.hpp"

namespace arx
{

    Event eventFromEventName(const string_t &event_name)
    {
        if (event_name == "AchievementEarned")
        {
            return Event::AchievementEarned;
        }
        if (event_name == "BattleRankUp")
        {
            return Event::BattleRankUp;
        }
        if (event_name == "ContinentLock")
        {
            return Event::ContinentLock;
        }
        if (event_name == "ContinentUnlock")
        {
            return Event::ContinentUnlock;
        }
        if (event_name == "Death")
        {
            return Event::Death;
        }
        if (event_name == "FacilityControl")
        {
            return Event::FacilityControl;
        }
        if (event_name == "GainExperience")
        {
            return Event::GainExperience;
        }
        if (event_name == "ItemAdded")
        {
            return Event::ItemAdded;
        }
        if (event_name == "MetagameEvent")
        {
            return Event::MetagameEvent;
        }
        if (event_name == "PlayerFacilityCapture")
        {
            return Event::PlayerFacilityCapture;
        }
        if (event_name == "PlayerFacilityDefend")
        {
            return Event::PlayerFacilityDefend;
        }
        if (event_name == "PlayerLogin")
        {
            return Event::PlayerLogin;
        }
        if (event_name == "PlayerLogout")
        {
            return Event::PlayerLogout;
        }
        if (event_name == "SkillAdded")
        {
            return Event::SkillAdded;
        }
        if (event_name == "VehicleDestroy")
        {
            return Event::VehicleDestroy;
        }
        return Event::Unknown;
    }

    string_t eventToEventName(const Event &event)
    {
        switch (event)
        {
        case Event::AchievementEarned:
            return "AchievementEarned";
        case Event::BattleRankUp:
            return "BattleRankUp";
        case Event::ContinentLock:
            return "ContinentLock";
        case Event::ContinentUnlock:
            return "ContinentUnlock";
        case Event::Death:
            return "Death";
        case Event::FacilityControl:
            return "FacilityControl";
        case Event::GainExperience:
            return "GainExperience";
        case Event::ItemAdded:
            return "ItemAdded";
        case Event::MetagameEvent:
            return "MetagameEvent";
        case Event::PlayerFacilityCapture:
            return "PlayerFacilityCapture";
        case Event::PlayerFacilityDefend:
            return "PlayerFacilityDefend";
        case Event::PlayerLogin:
            return "PlayerLogin";
        case Event::PlayerLogout:
            return "PlayerLogout";
        case Event::SkillAdded:
            return "SkillAdded";
        case Event::VehicleDestroy:
            return "VehicleDestroy";
        default:
            return "Unknown";
        }
    }

    bool isEventCharacterCentric(const Event &event)
    {
        switch (event)
        {
        case Event::AchievementEarned:
        case Event::BattleRankUp:
        case Event::Death:
        case Event::GainExperience:
        case Event::ItemAdded:
        case Event::PlayerFacilityCapture:
        case Event::PlayerFacilityDefend:
        case Event::PlayerLogin:
        case Event::PlayerLogout:
        case Event::SkillAdded:
        case Event::VehicleDestroy:
            return true;
        default:
            return false;
        }
    }

    bool isEventWorldCentric(const Event &event)
    {
        switch (event)
        {
        case Event::ContinentLock:
        case Event::ContinentUnlock:
        case Event::FacilityControl:
        case Event::MetagameEvent:
        case Event::PlayerFacilityCapture:
        case Event::PlayerFacilityDefend:
            return true;
        default:
            return false;
        }
    }

} // namespace arx
