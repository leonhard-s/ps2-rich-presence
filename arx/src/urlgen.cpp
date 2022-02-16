// Copyright 2022 Leonhard S.

#include "arx/urlgen.hpp"

#include <string>

namespace arx
{

    std::string getScheme()
    {
        return "https";
    }

    std::string getHost()
    {
        return "census.daybreakgames.com";
    }

    std::string generateCensusPath(const std::string &service_id,
                                   const std::string &format,
                                   const std::string &verb,
                                   const std::string &namespace_,
                                   const std::string &collection)
    {
        std::string path = "/" + service_id;
        if (format != "json")
        {
            path += "/" + format;
        }
        path += "/" + verb + "/" + namespace_;
        if (!collection.empty())
        {
            path += "/" + collection;
        }
        return path;
    }

} // namespace arx
