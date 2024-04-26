// Copyright 2022 Leonhard S.

#pragma once

#include <string>
#include <vector>

namespace arx {

enum class SearchModifier {
    EQUAL_TO,
    NOT_EQUAL_TO,
    GREATER_THAN,
    GREATER_THAN_OR_EQUAL_TO,
    LESS_THAN,
    LESS_THAN_OR_EQUAL_TO,
    CONTAINS,
    STARTS_WITH,
};

SearchModifier modifierFromData(std::string_view data);

std::string serialiseModifier(SearchModifier modifier);

class SearchTerm {
public:
    SearchTerm();
    SearchTerm(
        std::string_view field,
        std::string_view value,
        SearchModifier modifier = SearchModifier::EQUAL_TO);

    SearchTerm static createFromValue(
        std::string_view field,
        std::string_view value);

    std::pair<std::string, std::string> asQueryItem() const;
    std::string serialise() const;

private:
    std::string field_;
    std::string value_;
    SearchModifier modifier_;
};

std::string join(
    const std::vector<std::string>& strings,
    std::string_view delimiter);

} // namespace arx
