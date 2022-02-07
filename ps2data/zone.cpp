// Copyright 2022 Leonhard S.

#include "zone.hpp"

#include <string>

#include "id-types.hpp"

namespace ps2
{

    int zone_from_zone_id(ZoneId zone_id, Zone &zone)
    {
        switch (zone_id)
        {
        case 2:
            zone = Zone::Indar;
            break;
        case 4:
            zone = Zone::Hossin;
            break;
        case 6:
            zone = Zone::Amerish;
            break;
        case 8:
            zone = Zone::Esamir;
            break;
        case 14:
            zone = Zone::Koltyr;
            break;
        case 96: // NC
        case 97: // TR
        case 98: // VS
            zone = Zone::VrTraining;
            break;
        case 344:
            zone = Zone::Oshur;
            break;
        case 361:
            zone = Zone::Desolation;
            break;
        case 362:
            zone = Zone::Sanctuary;
            break;
        case 364:
            zone = Zone::Tutorial;
            break;
        default:
            return -1;
        }
        return 0;
    }

    int zone_to_display_name(Zone zone, std::string &display_name)
    {
        switch (zone)
        {
        case Zone::Indar:
            display_name = "Indar";
            break;
        case Zone::Hossin:
            display_name = "Hossin";
            break;
        case Zone::Amerish:
            display_name = "Amerish";
            break;
        case Zone::Esamir:
            display_name = "Esamir";
            break;
        case Zone::Oshur:
            display_name = "Oshur";
            break;
        case Zone::Sanctuary:
            display_name = "Sanctuary";
            break;
        default:
            return -1;
        }
        return 0;
    }

} // namespace ps2
