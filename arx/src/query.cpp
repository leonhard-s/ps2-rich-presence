// Copyright 2022 Leonhard S.

#include "arx/query.hpp"

#include <algorithm>
#include <iterator>
#include <initializer_list>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "arx/urlgen.hpp"

namespace arx {

std::vector<JoinData> SupportsJoin::getJoins() const {
    return joins;
}

void SupportsJoin::setJoins(std::initializer_list<JoinData> join_list) {
    this->joins = join_list;
}

void SupportsJoin::addJoin(const JoinData& join) {
    joins.push_back(join);
}

JoinData::JoinData()
    : collection_{ "" }
    , on_{ "" }
    , to_{ "" }
    , list_{ false }
    , show_{}
    , hide_{}
    , inject_at_{ "" }
    , terms_{}
    , outer_{ true } {}

JoinData::JoinData(
    const std::string& collection,
    const std::string& on,
    const std::string& to,
    bool list,
    const std::initializer_list<std::string>& show,
    const std::initializer_list<std::string>& hide,
    const std::string& inject_at,
    const std::initializer_list<SearchTerm>& terms,
    bool outer
)
    : collection_{ collection }
    , on_{ on }
    , to_{ to }
    , list_{ list }
    , show_{ show }
    , hide_{ hide }
    , inject_at_{ inject_at }
    , terms_{ terms }
    , outer_{ outer } {}

void JoinData::setFieldNames(const std::string& field) {
    this->on_ = field;
    this->to_ = field;
}

void JoinData::setFieldNames(
    const std::string& parent_field,
    const std::string& child_field
) {
    this->on_ = parent_field;
    this->to_ = child_field;
}

std::string JoinData::serialise() const {
    std::stringstream ss;
    ss << collection_;
    if (!on_.empty()) {
        ss << "^on:" << on_;
    }
    if (!to_.empty()) {
        ss << "^to:" << to_;
    }
    if (list_) {
        ss << "^list:1";
    }
    if (!show_.empty()) {
        ss << "^show:" << arx::join(show_, "'");
    }
    if (!hide_.empty()) {
        ss << "^hide:" << arx::join(hide_, "'");
    }
    if (!inject_at_.empty()) {
        ss << "^inject_at:" << inject_at_;
    }
    if (!terms_.empty()) {
        std::vector<std::string> terms_strings;
        terms_strings.reserve(terms_.size());
        std::transform(terms_.begin(), terms_.end(),
            std::back_inserter(terms_strings),
            [](const auto& term) { return term.serialise(); });
        ss << "^terms:" << arx::join(terms_strings, "'");
    }
    if (!outer_) {
        ss << "^outer:0";
    }
    if (!joins.empty()) {
        std::vector<std::string> joins_strings;
        joins_strings.reserve(joins.size());
        std::transform(joins.begin(), joins.end(),
            std::back_inserter(joins_strings),
            [](const auto& join) { return join.serialise(); });
        ss << "(" << arx::join(joins_strings, ",") << ")";
    }
    return ss.str();
}

TreeData::TreeData()
    : field_{ "" }
    , list_{ false }
    , prefix_{ "" }
    , start_{ "" } {}

TreeData::TreeData(
    const std::string& field,
    bool list,
    const std::string& prefix,
    const std::string& start
)
    : field_{ field }
    , list_{ list }
    , prefix_{ prefix }
    , start_{ start } {}

std::string TreeData::serialise() const {
    std::stringstream ss;
    ss << field_;
    if (!prefix_.empty()) {
        ss << "^prefix:" << prefix_;
    }
    if (list_) {
        ss << "^list:1";
    }
    if (!start_.empty()) {
        ss << "^start:" << start_;
    }
    return ss.str();
}

Query::Query(
    const std::string& collection,
    const std::string& service_id
)
    : SupportsJoin()
    , service_id_{ service_id }
    , format_{ "json" }
    , verb_{ "get" }
    , namespace_{ "ps2" }
    , collection_{ collection }
    , show_{}
    , hide_{}
    , sort_{}
    , has_{}
    , resolve_{}
    , case_{ true }
    , limit_{ 1 }
    , limit_per_db_{ 1 }
    , start_{ 0 }
    , include_null_{ false }
    , lang_{ "" }
    , tree_{}
    , timing_{ false }
    , exact_match_first_{ false }
    , distinct_{ "" },
    retry_{ true } {}

Query::Query(const Query& other)
    : SupportsJoin(other)
    , service_id_{ other.service_id_ }
    , format_{ other.format_ }
    , verb_{ other.verb_ }
    , namespace_{ other.namespace_ }
    , collection_{ other.collection_ }
    , show_{ other.show_ }
    , hide_{ other.hide_ }
    , sort_{ other.sort_ }
    , has_{ other.has_ }
    , resolve_{ other.resolve_ }
    , case_{ other.case_ }
    , limit_{ other.limit_ }
    , limit_per_db_{ other.limit_per_db_ }
    , start_{ other.start_ }
    , include_null_{ other.include_null_ }
    , lang_{ other.lang_ }
    , timing_{ other.timing_ }
    , exact_match_first_{ other.exact_match_first_ }
    , distinct_{ other.distinct_ }
    , retry_{ other.retry_ } {
    if (other.tree_) {
        tree_ = std::make_unique<TreeData>(*other.tree_);
    }
}

Query& Query::operator=(const Query& other) {
    if (this != &other) {
        SupportsJoin::operator=(other);
        service_id_ = other.service_id_;
        format_ = other.format_;
        verb_ = other.verb_;
        namespace_ = other.namespace_;
        collection_ = other.collection_;
        show_ = other.show_;
        hide_ = other.hide_;
        sort_ = other.sort_;
        has_ = other.has_;
        resolve_ = other.resolve_;
        case_ = other.case_;
        limit_ = other.limit_;
        limit_per_db_ = other.limit_per_db_;
        start_ = other.start_;
        include_null_ = other.include_null_;
        lang_ = other.lang_;
        timing_ = other.timing_;
        exact_match_first_ = other.exact_match_first_;
        distinct_ = other.distinct_;
        retry_ = other.retry_;
        if (other.tree_) {
            tree_ = std::make_unique<TreeData>(*other.tree_);
        }
    }
    return *this;
}

std::string Query::getServiceId() const {
    return service_id_;
}

void Query::setServiceId(const std::string& service_id) {
    service_id_ = service_id;
}

std::string Query::getFormat() const {
    return format_;
}

void Query::setFormat(const std::string& format) {
    format_ = format;
}

std::string Query::getVerb() const {
    return verb_;
}

void Query::setVerb(const std::string& verb) {
    verb_ = verb;
}

std::string Query::getNamespace() const {
    return namespace_;
}

void Query::setNamespace(const std::string& game) {
    this->namespace_ = game;
}

std::string Query::getCollection() const {
    return collection_;
}

void Query::setCollection(const std::string& collection) {
    collection_ = collection;
}

std::vector<SearchTerm> Query::getTerms() const {
    return terms_;
}

void Query::setTerms(const std::initializer_list<SearchTerm>& terms) {
    terms_ = terms;
}

void Query::addTerm(const SearchTerm& term) {
    terms_.push_back(term);
}

std::vector<std::string> Query::getShow() const {
    return show_;
}

void Query::setShow(const std::initializer_list<std::string>& show) {
    show_ = show;
}

std::vector<std::string> Query::getHide() const {
    return hide_;
}

void Query::setHide(const std::initializer_list<std::string>& hide) {
    hide_ = hide;
}

std::vector<std::string> Query::getSort() const {
    return sort_;
}

void Query::setSort(const std::initializer_list<std::string>& sort) {
    sort_ = sort;
}

std::vector<std::string> Query::getHas() const {
    return has_;
}

void Query::setHas(const std::initializer_list<std::string>& has) {
    has_ = has;
}

std::vector<std::string> Query::getResolve() const {
    return resolve_;
}

void Query::setResolve(const std::initializer_list<std::string>& resolve) {
    resolve_ = resolve;
}

bool Query::getCase() const {
    return case_;
}

void Query::setCase(bool value) {
    this->case_ = value;
}

int Query::getLimit() const {
    return limit_;
}

void Query::setLimit(int limit) {
    limit_ = limit;
}

int Query::getLimitPerDb() const {
    return limit_per_db_;
}

void Query::setLimitPerDb(int limit_per_db) {
    limit_per_db_ = limit_per_db;
}

int Query::getStart() const {
    return start_;
}

void Query::setStart(int start) {
    start_ = start;
}

bool Query::getIncludeNull() const {
    return include_null_;
}

void Query::setIncludeNull(bool include_null) {
    include_null_ = include_null;
}

std::string Query::getLang() const {
    return lang_;
}

void Query::setLang(const std::string& lang) {
    lang_ = lang;
}

const TreeData* Query::getTree() const {
    return tree_.get();
}

void Query::setTree() {
    tree_.reset();
}

void Query::setTree(const TreeData& tree) {
    *tree_ = tree;
}

bool Query::getTiming() const {
    return timing_;
}

void Query::setTiming(bool timing) {
    timing_ = timing;
}

bool Query::getExactMatchFirst() const {
    return exact_match_first_;
}

void Query::setExactMatchFirst(bool exact_match_first) {
    exact_match_first_ = exact_match_first;
}

std::string Query::getDistinct() const {
    return distinct_;
}

void Query::setDistinct(const std::string& distinct) {
    distinct_ = distinct;
}

bool Query::getRetry() const {
    return retry_;
}

void Query::setRetry(bool retry) {
    retry_ = retry;
}

std::string Query::getUrl() const {
    std::stringstream url;
    url << getScheme() << "://" << getHost() << getPath();
    auto query_items = getQuery();
    std::transform(query_items.begin(), query_items.end(),
        std::ostream_iterator<std::string>(url),
        [query_items](const std::pair<std::string, std::string>& item) {
            char delim = '&';
            if (item == query_items.front()) {
                delim = '?';
            }
            std::stringstream ss;
            ss << delim << item.first << "=" << item.second;
            return ss.str();
        });
    return url.str();
}

std::string Query::getScheme() const {
    return arx::getScheme();
}

std::string Query::getHost() const {
    return arx::getHost();
}

std::string Query::getPath() const {
    return generateCensusPath(
        service_id_, format_, verb_, namespace_, collection_);
}

std::vector<std::pair<std::string, std::string>> Query::getQuery() const {
    return arx::getQueryItems(this);
}

} // namespace arx
