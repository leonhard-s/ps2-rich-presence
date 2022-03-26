// Copyright 2022 Leonhard S.

#ifndef ARX_PS2_TYPES_HPP
#define ARX_PS2_TYPES_HPP

#include <cstdint>

namespace arx
{

    using ability_id_t = uint_fast32_t;
    using ability_type_id_t = uint_fast8_t;
    using achievement_id_t = uint_fast32_t;
    using armor_facing_id_t = uint_fast8_t;
    using armor_info_id_t = uint_fast16_t;
    using character_id_t = uint_least64_t;
    using currency_id_t = uint_fast8_t;
    using directive_id_t = uint_fast16_t;
    using directive_tier_id_t = uint_fast8_t;
    using directive_tree_id_t = uint_fast8_t;
    using directive_tree_category_id_t = uint_fast8_t;
    using effect_id_t = uint_fast32_t;
    using effect_type_id_t = uint_fast8_t;
    using experience_id_t = uint_fast16_t;
    using facility_id_t = uint_fast32_t;
    using facility_type_id_t = uint_fast8_t;
    using faction_id_t = uint_fast8_t;
    using fire_group_id_t = uint_fast32_t;
    using fire_mode_id_t = uint_fast32_t;
    using fire_mode_type_id_t = uint_fast8_t;
    using image_id_t = uint_fast32_t;
    using image_set_id_t = uint_fast32_t;
    using item_id_t = uint_fast32_t;
    using item_category_id_t = uint_fast16_t;
    using item_type_id_t = uint_fast8_t;
    using loadout_id_t = uint_fast8_t;
    using map_region_id_t = uint_fast16_t;
    using marketing_bundle_id_t = uint_fast32_t;
    using metagame_event_id_t = uint_fast16_t;
    using metagame_event_state_id_t = uint_fast8_t;
    using objective_id_t = uint_fast16_t;
    using objective_set_id_t = uint_fast16_t;
    using objective_group_id_t = uint_fast16_t;
    using objective_type_id_t = uint_fast8_t;
    using outfit_id_t = uint_least64_t;
    using player_state_id_t = uint_fast8_t;
    using player_state_group_t = uint_fast32_t;
    using profile_id_t = uint_fast16_t;
    using projectile_id_t = uint_fast32_t;
    using projectile_flight_type_id_t = uint_fast8_t;
    using region_id_t = uint_fast16_t;
    using resist_info_id_t = uint_fast16_t;
    using resist_type_id_t = uint_fast8_t;
    using resource_type_id_t = uint_fast16_t;
    using reward_id_t = uint_fast16_t;
    using reward_group_id_t = uint_fast32_t;
    using reward_set_id_t = uint_fast16_t;
    using reward_type_id_t = uint_fast8_t;
    using skill_id_t = uint_fast16_t;
    using skill_category_id_t = uint_fast16_t;
    using skill_set_id_t = uint_fast16_t;
    using target_type_id_t = uint_fast8_t;
    using title_id_t = uint_fast16_t;
    using vehicle_id_t = uint_fast16_t;
    using weapon_id_t = uint_fast32_t;
    using zone_id_t = uint_fast32_t;
    using zone_effect_id_t = uint_fast32_t;
    using zone_effect_type_id_t = uint_fast16_t;
    using world_id_t = uint_fast8_t;

} // namespace arx

#endif // ARX_PS2_TYPES_HPP
