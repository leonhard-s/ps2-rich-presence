// Copyright 2022 Leonhard S.

#ifndef ARX_TYPES_HPP
#define ARX_TYPES_HPP

#include <json.hpp>

namespace arx
{
    using json_object = nlohmann::json;

    using json_array = json_object::array_t;

    using json_value = json_object::value_t;

    using json_string = json_object::string_t;

} // namespace arx

#endif // ARX_TYPES_HPP
