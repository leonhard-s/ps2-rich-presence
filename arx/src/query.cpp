// Copyright 2022 Leonhard S.

#include "arx/query.hpp"

#include <initializer_list>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "arx/urlgen.hpp"

namespace arx {

std::vector<JoinData> SupportsJoin::getJoins() const
{
    return joins;
}

void SupportsJoin::setJoins(std::initializer_list<JoinData> join_list)
{
    this->joins = join_list;
}

JoinData& SupportsJoin::addJoin(JoinData& join)
{
    joins.push_back(join);
    return join;
}

JoinData::JoinData()
    : collection{ "" }, on{ "" }, to{ "" }, list{ false }, show{}, hide{},
    inject_at{ "" }, terms{}, outer{ true } {}

JoinData::JoinData(const std::string& collection,
    const std::string& on,
    const std::string& to,
    bool list,
    const std::initializer_list<std::string>& show,
    const std::initializer_list<std::string>& hide,
    const std::string& inject_at,
    const std::initializer_list<SearchTerm>& terms,
    bool outer)
    : collection{ collection }, on{ on }, to{ to }, list{ list }, show{ show },
    hide{ hide }, inject_at{ inject_at }, terms{ terms }, outer{ outer } {}

void JoinData::setFieldNames(const std::string& field)
{
    this->on = field;
    this->to = field;
}

void JoinData::setFieldNames(const std::string& parent_field,
    const std::string& child_field)
{
    this->on = parent_field;
    this->to = child_field;
}

std::string JoinData::serialise() const
{
    std::string str = collection;
    if (!on.empty())
    {
        str += "^on:" + on;
    }
    if (!to.empty())
    {
        str += "^to:" + to;
    }
    if (list)
    {
        str += "^list:1";
    }
    if (!show.empty())
    {
        str += "^show:" + arx::join(show, "'");
    }
    if (!hide.empty())
    {
        str += "^hide:" + arx::join(hide, "'");
    }
    if (!inject_at.empty())
    {
        str += "^inject_at:" + inject_at;
    }
    if (!terms.empty())
    {
        std::vector<std::string> terms_strings;
        for (const auto& term : terms)
        {
            terms_strings.push_back(term.serialise());
        }
        str += "^terms:" + arx::join(terms_strings, "'");
    }
    if (!outer)
    {
        str += "^outer:0";
    }
    if (!joins.empty())
    {
        std::vector<std::string> joins_strings;
        for (const auto& join : joins)
        {
            joins_strings.push_back(join.serialise());
        }
        str += "(" + arx::join(joins_strings, ",") + ")";
    }
    return str;
}

TreeData::TreeData()
    : field{ "" }, list{ false }, prefix{ "" }, start{ "" } {}

TreeData::TreeData(const std::string& field,
    bool list,
    const std::string& prefix,
    const std::string& start)
    : field{ field }, list{ list }, prefix{ prefix }, start{ start } {}

std::string TreeData::serialise() const
{
    std::string str = field;
    if (!prefix.empty())
    {
        str += "^prefix:" + prefix;
    }
    if (list)
    {
        str += "^list:1";
    }
    if (!start.empty())
    {
        str += "^start:" + start;
    }
    return str;
}

Query::Query(const std::string& collection,
    const std::string& service_id)
    : SupportsJoin(), service_id_{ service_id }, format_{ "json" },
    verb_{ "get" }, namespace_{ "ps2" }, collection_{ collection }, show_{},
    hide_{}, sort_{}, has_{}, resolve_{}, case_{ true }, limit_{ 1 },
    limit_per_db_{ 1 }, start_{ 0 }, include_null_{ false }, lang_{ "" },
    tree_{}, timing_{ false }, exact_match_first_{ false }, distinct_{ "" },
    retry_{ true } {}

std::string Query::getServiceId() const
{
    return service_id_;
}

void Query::setServiceId(const std::string& service_id)
{
    service_id_ = service_id;
}

std::string Query::getFormat() const
{
    return format_;
}

void Query::setFormat(const std::string& format)
{
    format_ = format;
}

std::string Query::getVerb() const
{
    return verb_;
}

void Query::setVerb(const std::string& verb)
{
    verb_ = verb;
}

std::string Query::getNamespace() const
{
    return namespace_;
}

void Query::setNamespace(const std::string& game)
{
    this->namespace_ = game;
}

std::string Query::getCollection() const
{
    return collection_;
}

void Query::setCollection(const std::string& collection)
{
    collection_ = collection;
}

std::vector<SearchTerm> Query::getTerms() const
{
    return terms_;
}

void Query::setTerms(const std::initializer_list<SearchTerm>& terms)
{
    terms_ = terms;
}

void Query::addTerm(const SearchTerm& term)
{
    terms_.push_back(term);
}

std::vector<std::string> Query::getShow() const
{
    return show_;
}

void Query::setShow(const std::initializer_list<std::string>& show)
{
    show_ = show;
}

std::vector<std::string> Query::getHide() const
{
    return hide_;
}

void Query::setHide(const std::initializer_list<std::string>& hide)
{
    hide_ = hide;
}

std::vector<std::string> Query::getSort() const
{
    return sort_;
}

void Query::setSort(const std::initializer_list<std::string>& sort)
{
    sort_ = sort;
}

std::vector<std::string> Query::getHas() const
{
    return has_;
}

void Query::setHas(const std::initializer_list<std::string>& has)
{
    has_ = has;
}

std::vector<std::string> Query::getResolve() const
{
    return resolve_;
}

void Query::setResolve(const std::initializer_list<std::string>& resolve)
{
    resolve_ = resolve;
}

bool Query::getCase() const
{
    return case_;
}

void Query::setCase(bool value)
{
    this->case_ = value;
}

int Query::getLimit() const
{
    return limit_;
}

void Query::setLimit(int limit)
{
    limit_ = limit;
}

int Query::getLimitPerDb() const
{
    return limit_per_db_;
}

void Query::setLimitPerDb(int limit_per_db)
{
    limit_per_db_ = limit_per_db;
}

int Query::getStart() const
{
    return start_;
}

void Query::setStart(int start)
{
    start_ = start;
}

bool Query::getIncludeNull() const
{
    return include_null_;
}

void Query::setIncludeNull(bool include_null)
{
    include_null_ = include_null;
}

std::string Query::getLang() const
{
    return lang_;
}

void Query::setLang(const std::string& lang)
{
    lang_ = lang;
}

const TreeData* Query::getTree() const
{
    return tree_.get();
}

void Query::setTree()
{
    tree_.reset();
}

void Query::setTree(TreeData& tree)
{
    *tree_ = tree;
}

bool Query::getTiming() const
{
    return timing_;
}

void Query::setTiming(bool timing)
{
    timing_ = timing;
}

bool Query::getExactMatchFirst() const
{
    return exact_match_first_;
}

void Query::setExactMatchFirst(bool exact_match_first)
{
    exact_match_first_ = exact_match_first;
}

std::string Query::getDistinct() const
{
    return distinct_;
}

void Query::setDistinct(const std::string& distinct)
{
    distinct_ = distinct;
}

bool Query::getRetry() const
{
    return retry_;
}

void Query::setRetry(bool retry)
{
    retry_ = retry;
}

std::string Query::getUrl() const
{
    std::string url = getScheme() + "://" + getHost() + getPath();
    auto query_items = getQuery();
    for (auto it = query_items.begin(); it != query_items.end(); it++)
    {
        if (it == query_items.begin())
        {
            url += "?";
        }
        else
        {
            url += "&";
        }
        url += it->first + "=" + it->second;
    }
    return url;
}

std::string Query::getScheme() const
{
    return arx::getScheme();
}

std::string Query::getHost() const
{
    return arx::getHost();
}

std::string Query::getPath() const
{
    return generateCensusPath(service_id_, format_, verb_, namespace_, collection_);
}

std::vector<std::pair<std::string, std::string>> Query::getQuery() const
{
    return arx::getQueryItems(this);
}

} // namespace arx
