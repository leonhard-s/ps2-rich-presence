// Copyright 2022 Leonhard S.

#pragma once

#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "query.hpp"

namespace arx {

std::string_view getScheme();
std::string_view getHost();

std::string generateCensusPath(
    const std::string& service_id,
    const std::string& format = "json",
    const std::string& verb = "get",
    const std::string& ns = "ps2",
    const std::string& collection = "");

std::vector<std::pair<std::string, std::string>> getQueryItems(
    const Query* query);

} // namespace arx
