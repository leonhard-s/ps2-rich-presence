// Copyright 2022 Leonhard S.

#ifndef ARX_URLGEN_HPP
#define ARX_URLGEN_HPP

#include <string>

namespace arx
{

    std::string getScheme();

    std::string getHost();

    std::string generateCensusPath(const std::string &service_id,
                                   const std::string &format = "json",
                                   const std::string &verb = "get",
                                   const std::string &namespace_ = "ps2",
                                   const std::string &collection = "");

} // namespace arx

#endif // ARX_URLGEN_HPP
