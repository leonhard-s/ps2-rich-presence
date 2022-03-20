// Copyright 2022 Leonhard S.

#include "arx/ess/endpoint.hpp"

#include <sstream>
#include <string>

#include "arx/types.hpp"

namespace
{

    arx::string_t environmentToString(const arx::Environment &environment)
    {
        switch (environment)
        {
        case arx::Environment::PS2:
            return "ps2";
        case arx::Environment::PS2PS4EU:
            return "ps2ps4eu";
        case arx::Environment::PS2PS4US:
            return "ps2ps4us";
        }
        return "none";
    }

} // namespace

namespace arx
{

    string_t getEndpointUrl(const string_t &service_id,
                            const Environment &environment)
    {
        std::stringstream ss;
        ss << "wss://push.planetside2.com/streaming?environment="
           << environmentToString(environment)
           << "&service-id="
           << service_id;
        return ss.str();
    }

} // namespace arx
