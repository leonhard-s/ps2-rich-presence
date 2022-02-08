// Copyright 2022 Leonhard S.

#ifndef PS2RPC_APPDATA_ASSETS_HPP
#define PS2RPC_APPDATA_ASSETS_HPP

#include <string>

#include "ps2.hpp"

namespace assets
{

    int imageKeyFromClass(ps2::Class class_, std::string &image_key);

    int imageKeyFromVehicle(ps2::Vehicle vehicle, std::string &image_key);

    int imageKeyFromZone(ps2::Zone zone, std::string &image_key);

} // namespace assets

#endif // PS2RPC_APPDATA_ASSETS_HPP
