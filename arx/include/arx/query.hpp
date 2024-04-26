// Copyright 2022 Leonhard S.

#pragma once

#include <initializer_list>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "arx/support.hpp"

namespace arx {

struct JoinData;

class SupportsJoin {
public:
    std::vector<JoinData> getJoins() const;
    void setJoins(std::initializer_list<JoinData> joins_list);
    void addJoin(const JoinData& join);

protected:
    std::vector<JoinData> joins;
};

struct JoinData : public SupportsJoin {
    JoinData();
    explicit JoinData(
        std::string_view collection,
        std::string_view on = "",
        std::string_view to = "",
        bool list = false,
        const std::initializer_list<std::string>& show = {},
        const std::initializer_list<std::string>& hide = {},
        std::string_view inject_at = "",
        const std::initializer_list<SearchTerm>& terms = {},
        bool outer = true);

    void setFieldNames(std::string_view field);
    void setFieldNames(
        std::string_view parent_field,
        std::string_view child_field);

    std::string serialise() const;

    std::string collection_;
    std::string on_;
    std::string to_;
    bool list_;
    std::vector<std::string> show_;
    std::vector<std::string> hide_;
    std::string inject_at_;
    std::vector<SearchTerm> terms_;
    bool outer_;
};

struct TreeData {
    TreeData();
    explicit TreeData(
        std::string_view field,
        bool list = false,
        std::string_view prefix = "",
        std::string_view start = "");

    std::string serialise() const;

    std::string field_;
    bool list_;
    std::string prefix_;
    std::string start_;
};

class Query : public SupportsJoin {
public:
    explicit Query(
        std::string_view collection = "",
        std::string_view service_id = "s:example");
    Query(const Query& other);
    Query(Query&& other) noexcept = default;

    Query& operator=(const Query& other);
    Query& operator=(Query&& other) noexcept = default;

    // Request format / path configuration

    std::string getServiceId() const;
    void setServiceId(std::string_view service_id);
    std::string getFormat() const;
    void setFormat(std::string_view format);
    std::string getVerb() const;
    void setVerb(std::string_view verb);
    std::string getNamespace() const;
    void setNamespace(std::string_view game);
    std::string getCollection() const;
    void setCollection(std::string_view collection);

    // Query terms

    std::vector<SearchTerm> getTerms() const;
    void setTerms(const std::initializer_list<SearchTerm>& terms);
    void addTerm(const SearchTerm& term);

    // Query commands

    std::vector<std::string> getShow() const;
    void setShow(const std::initializer_list<std::string>& show);
    std::vector<std::string> getHide() const;
    void setHide(const std::initializer_list<std::string>& hide);
    std::vector<std::string> getSort() const;
    void setSort(const std::initializer_list<std::string>& sort);
    std::vector<std::string> getHas() const;
    void setHas(const std::initializer_list<std::string>& has);
    std::vector<std::string> getResolve() const;
    void setResolve(const std::initializer_list<std::string>& resolve_list);
    bool getCase() const;
    void setCase(bool case_sensitive);
    int getLimit() const;
    void setLimit(int limit);
    int getLimitPerDb() const;
    void setLimitPerDb(int limit_per_db);
    int getStart() const;
    void setStart(int start);
    bool getIncludeNull() const;
    void setIncludeNull(bool include_null);
    std::string getLang() const;
    void setLang(std::string_view lang);
    // c:join functionality inherited from SupportsJoin parent class
    const TreeData* getTree() const;
    void setTree();
    void setTree(const TreeData& tree);
    void setTree(
        std::string_view field,
        bool list = false,
        std::string_view prefix = "",
        std::string_view start = "");
    bool getTiming() const;
    void setTiming(bool timing);
    bool getExactMatchFirst() const;
    void setExactMatchFirst(bool exact_match_first);
    std::string getDistinct() const;
    void setDistinct(std::string_view distinct);
    bool getRetry() const;
    void setRetry(bool retry);

    // Utilities

    std::string getUrl() const;

    static std::string_view getScheme();
    static std::string_view getHost();
    std::string getPath() const;
    std::vector<std::pair<std::string, std::string>> getQuery() const;

private:
    // Request format / path configuration
    std::string service_id_;
    std::string format_;
    std::string verb_;
    std::string namespace_;
    std::string collection_;

    // Search terms
    std::vector<SearchTerm> terms_;

    // Query commands
    std::vector<std::string> show_;
    std::vector<std::string> hide_;
    std::vector<std::string> sort_;
    std::vector<std::string> has_;
    std::vector<std::string> resolve_;
    bool case_;
    int limit_;
    int limit_per_db_;
    int start_;
    bool include_null_;
    std::string lang_;
    std::unique_ptr<TreeData> tree_;
    bool timing_;
    bool exact_match_first_;
    std::string distinct_;
    bool retry_;
};

} // namespace arx
