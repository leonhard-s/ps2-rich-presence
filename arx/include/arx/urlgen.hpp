// Copyright 2022 Leonhard S.

#pragma once

#include <string>
#include <utility>
#include <vector>

#include "query.hpp"

namespace arx {

std::string getScheme();

std::string getHost();

std::string generateCensusPath(
    const std::string& service_id,
    const std::string& format = "json",
    const std::string& verb = "get",
    const std::string& namespace_ = "ps2",
    const std::string& collection = "");

std::vector<std::pair<std::string, std::string>> getQueryItems(
    const Query* query);

} // namespace arx
