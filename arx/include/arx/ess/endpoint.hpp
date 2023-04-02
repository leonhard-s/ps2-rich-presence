// Copyright 2022 Leonhard S.

#pragma once

#include "arx/types.hpp"

namespace arx
{

    /**
     * Enumeration of the available event streaming environments.
     */
    enum class Environment
    {
        PS2,
        PS2PS4EU,
        PS2PS4US
    };

    /**
     * URL generator for the ESS endpoint.
     *
     * @param service_id Service ID identifying this application to the ESS.
     * @param environment Event streaming environment to connect to.
     * @return URL to connect to the ESS.
     */
    string_t getEndpointUrl(const string_t &service_id = "s:example",
                            const Environment &environment = Environment::PS2);

} // namespace arx
