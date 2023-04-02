// Copyright 2022 Leonhard S.

#pragma once

#include <initializer_list>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "arx/support.hpp"

namespace arx {

struct JoinData;

class SupportsJoin {
public:
    std::vector<JoinData> getJoins() const;
    void setJoins(std::initializer_list<JoinData> joins);
    void addJoin(const JoinData& join);

protected:
    std::vector<JoinData> joins;
};

struct JoinData: public SupportsJoin {
    JoinData();
    explicit JoinData(
        const std::string& collection,
        const std::string& on = "",
        const std::string& to = "",
        bool list = false,
        const std::initializer_list<std::string>& show = {},
        const std::initializer_list<std::string>& hide = {},
        const std::string& inject_at = "",
        const std::initializer_list<SearchTerm>& terms = {},
        bool outer = true);

    void setFieldNames(const std::string& field);
    void setFieldNames(
        const std::string& parent_field,
        const std::string& child_field);

    std::string serialise() const;

    std::string collection;
    std::string on;
    std::string to;
    bool list;
    std::vector<std::string> show;
    std::vector<std::string> hide;
    std::string inject_at;
    std::vector<SearchTerm> terms;
    bool outer;
};

struct TreeData {
    TreeData();
    explicit TreeData(
        const std::string& field,
        bool list = false,
        const std::string& prefix = "",
        const std::string& start = "");

    std::string serialise() const;

    std::string field;
    bool list;
    std::string prefix;
    std::string start;
};

class Query: public SupportsJoin {
public:
    explicit Query(
        const std::string& collection = "",
        const std::string& service_id = "s:example");

    // Request format / path configuration

    std::string getServiceId() const;
    void setServiceId(const std::string& service_id);
    std::string getFormat() const;
    void setFormat(const std::string& format);
    std::string getVerb() const;
    void setVerb(const std::string& verb);
    std::string getNamespace() const;
    void setNamespace(const std::string& namespace_);
    std::string getCollection() const;
    void setCollection(const std::string& collection);

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
    void setLang(const std::string& lang);
    // c:join functionality inherited from SupportsJoin parent class
    const TreeData* getTree() const;
    void setTree();
    void setTree(const TreeData& tree);
    void setTree(
        const std::string& field,
        bool list = false,
        const std::string& prefix = "",
        const std::string& start = "");
    bool getTiming() const;
    void setTiming(bool timing);
    bool getExactMatchFirst() const;
    void setExactMatchFirst(bool exact_match_first);
    std::string getDistinct() const;
    void setDistinct(const std::string& distinct);
    bool getRetry() const;
    void setRetry(bool retry);

    // Utilities

    std::string getUrl() const;

    std::string getScheme() const;
    std::string getHost() const;
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
