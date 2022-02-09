// Copyright 2022 Leonhard S.

#ifndef PS2Data_ID_TYPES_HPP
#define PS2Data_ID_TYPES_HPP

#include <cstdint>

namespace ps2
{
    // The values 0 and -1 (i.e., the maximum value for uint) are reserved for
    // "none" and "null" respectively.

    //  8 bytes per ID, can store Ids up to 255
    using FactionId = uint_fast8_t;
    using LoadoutId = uint_fast8_t;
    using ProfileId = uint_fast8_t;
    using WorldId = uint_fast8_t;

    // 16 bytes per ID, can store Ids up to 65'535
    using VehicleId = uint_fast16_t;
    using ZoneId = uint_fast16_t;

    // 32 bytes per ID, can store Ids up to 4'294'967'295

    // 64 bytes per ID, can store Ids up to 18'446'744'073'709'551'615
    using CharacterId = uint_least64_t;
    using OutfitId = uint_least64_t;

} // namespace ps2

#endif // PS2Data_ID_TYPES_HPP
