// Copyright 2022 Leonhard S.

#include "arx/urlgen.hpp"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "arx/support.hpp"
#include "arx/query.hpp"

namespace arx {

std::string_view getScheme() {
    static const auto* const value = "https";
    return value;
}

std::string_view getHost() {
    static const auto* const value = "census.daybreakgames.com";
    return value;
}

std::string generateCensusPath(
    const std::string& service_id,
    const std::string& format,
    const std::string& verb,
    const std::string& ns,
    const std::string& collection
) {
    std::stringstream path;
    path << "/" << service_id;
    if (format != "json") {
        path << "/" << format;
    }
    path << "/" << verb << "/" << ns;
    if (!collection.empty()) {
        path << "/" << collection;
    }
    return path.str();
}

std::vector<std::pair<std::string, std::string>> getQueryItems(
    const Query* query
) {
    std::vector<std::pair<std::string, std::string>> items;
    // Search terms
    auto terms = query->getTerms();
    std::transform(terms.begin(), terms.end(), std::back_inserter(items),
        [](const SearchTerm& term) { return term.asQueryItem(); });
    // Query commands
    auto show = query->getShow();
    if (!show.empty()) {
        items.emplace_back("c:show", arx::join(show, ","));
    }
    auto hide = query->getHide();
    if (!hide.empty()) {
        items.emplace_back("c:hide", arx::join(hide, ","));
    }
    auto sort = query->getSort();
    if (!sort.empty()) {
        items.emplace_back("c:sort", arx::join(sort, ","));
    }
    auto has = query->getHas();
    if (!has.empty()) {
        items.emplace_back("c:has", arx::join(has, ","));
    }
    auto resolve = query->getResolve();
    if (!resolve.empty()) {
        items.emplace_back("c:resolve", arx::join(resolve, ","));
    }
    if (!query->getCase()) {
        items.emplace_back("c:case", "0");
    }
    auto limit = query->getLimit();
    if (limit > 1) {
        items.emplace_back("c:limit", std::to_string(limit));
    }
    auto limit_per_db = query->getLimitPerDb();
    if (limit_per_db > 1) {
        items.emplace_back("c:limit_per_db", std::to_string(limit_per_db));
    }
    auto start = query->getStart();
    if (start > 0) {
        items.emplace_back("c:start", std::to_string(start));
    }
    if (query->getIncludeNull()) {
        items.emplace_back("c:include_null", "1");
    }
    auto lang = query->getLang();
    if (!lang.empty()) {
        items.emplace_back("c:lang", lang);
    }
    if (query->getTiming()) {
        items.emplace_back("c:timing", "1");
    }
    if (query->getExactMatchFirst()) {
        items.emplace_back("c:exact_match_first", "1");
    }
    auto distinct = query->getDistinct();
    if (!distinct.empty()) {
        items.emplace_back("c:distinct", distinct);
    }
    if (!query->getRetry()) {
        items.emplace_back("c:retry", "0");
    }
    const auto* const tree = query->getTree();
    if (tree != nullptr) {
        items.emplace_back("c:tree", tree->serialise());
    }
    auto joins = query->getJoins();
    if (!joins.empty()) {
        std::vector<std::string> join_strings;
        join_strings.reserve(joins.size());
        std::transform(joins.begin(), joins.end(),
            std::back_inserter(join_strings),
            [](const auto& join) { return join.serialise(); });
        items.emplace_back("c:join", arx::join(join_strings, ","));
    }
    return items;
}

} // namespace arx
