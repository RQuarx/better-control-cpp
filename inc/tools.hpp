#pragma once
#ifndef TOOLS_HPP_
#define TOOLS_HPP_

#include <string>


namespace Tools {
    /// The default directory for caching on the system
    static const std::string CACHE_DIR = std::string(std::getenv("HOME")) + "/.cache/better_control/";

    static const std::string INFO_DIR = CACHE_DIR + "info";

    /// Fetches the init system from /sbin/init
    /// @returns the init system in lowercase, or an empty string if not found
    auto get_init_system() -> std::string;
}

#endif /* tools.hpp */