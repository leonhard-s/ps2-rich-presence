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

SearchModifier modifierFromData(const std::string& data);

std::string serialiseModifier(SearchModifier modifier);

class SearchTerm {
public:
    SearchTerm();
    SearchTerm(
        const std::string& field,
        const std::string& value,
        SearchModifier modifier = SearchModifier::EQUAL_TO);

    SearchTerm static createFromValue(
        const std::string& field,
        const std::string& value);

    std::pair<std::string, std::string> asQueryItem() const;
    std::string serialise() const;

private:
    std::string field_;
    std::string value_;
    SearchModifier modifier_;
};

std::string join(
    const std::vector<std::string>& strings,
    const std::string& delimiter);

} // namespace arx
