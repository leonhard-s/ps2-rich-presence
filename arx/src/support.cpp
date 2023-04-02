// Copyright 2022 Leonhard S.

#include "arx/support.hpp"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

namespace arx {

SearchModifier modifierFromData(const std::string& data) {
    auto first_char = data.front();
    switch (first_char) {
    case '!':
        return SearchModifier::NOT_EQUAL_TO;
    case '>':
        return SearchModifier::GREATER_THAN;
    case ']':
        return SearchModifier::GREATER_THAN_OR_EQUAL_TO;
    case '<':
        return SearchModifier::LESS_THAN;
    case '[':
        return SearchModifier::LESS_THAN_OR_EQUAL_TO;
    case '*':
        return SearchModifier::CONTAINS;
    case '^':
        return SearchModifier::STARTS_WITH;
    default:
        return SearchModifier::EQUAL_TO;
    }
}

std::string serialiseModifier(SearchModifier modifier) {
    switch (modifier) {
    case SearchModifier::NOT_EQUAL_TO:
        return "!";
    case SearchModifier::GREATER_THAN:
        return ">";
    case SearchModifier::GREATER_THAN_OR_EQUAL_TO:
        return "]";
    case SearchModifier::LESS_THAN:
        return "<";
    case SearchModifier::LESS_THAN_OR_EQUAL_TO:
        return "[";
    case SearchModifier::CONTAINS:
        return "*";
    case SearchModifier::STARTS_WITH:
        return "^";
    default:
        return "";
    }
}

SearchTerm::SearchTerm()
    : field_{ "" }
    , value_{ "" }
    , modifier_{ SearchModifier::EQUAL_TO } {}

SearchTerm::SearchTerm(
    const std::string& field,
    const std::string& value,
    SearchModifier modifier
)
    : field_{ field }
    , value_{ value }
    , modifier_{ modifier } {}

SearchTerm SearchTerm::createFromValue(const std::string& field,
    const std::string& value) {
    // Check if the given value contains a modifier literal
    auto mod = modifierFromData(value);
    if (mod != SearchModifier::EQUAL_TO) {
        return SearchTerm{ field, value.substr(1), mod };
    }
    else {
        return SearchTerm{ field, value, mod };
    }
}

std::pair<std::string, std::string> SearchTerm::asQueryItem() const {
    return std::make_pair(field_, serialiseModifier(modifier_) + value_);
}

std::string SearchTerm::serialise() const {
    std::stringstream ss;
    ss << field_ << "=" << serialiseModifier(modifier_) << value_;
    return ss.str();
}

std::string join(
    const std::vector<std::string>& strings,
    const std::string& delimiter
) {
    std::stringstream joined;
    std::transform(strings.cbegin(), strings.cend(),
        std::ostream_iterator<std::string>(joined),
        [&strings, &delimiter](const std::string& str) {
            if (str == strings.front()) {
                return str;
            }
            return delimiter + str;
        });
    return joined.str();
}

} // namespace arx
