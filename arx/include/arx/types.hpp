// Copyright 2022 Leonhard S.

#ifndef ARX_TYPES_HPP
#define ARX_TYPES_HPP

#include <string>

#include <json.hpp>

namespace arx
{
    using string_t = std::string;

    using json_t = nlohmann::json;

    using json_array_t = json_t::array_t;

    using json_value_t = json_t::value_t;

    using json_string_t = json_t::string_t;

} // namespace arx

#endif // ARX_TYPES_HPP
