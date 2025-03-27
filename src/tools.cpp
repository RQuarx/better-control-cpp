#include "../inc/tools.hpp"

#include <filesystem>
#include <fstream>
#include <print>

#include "../inc/utils.hpp"


namespace Tools {
    auto
    get_init_system() -> std::string
    {
        std::filesystem::path init_path = std::filesystem::canonical("/sbin/init");

        std::filesystem::path info_path = INFO_DIR;
        if (Utils::check_line_exist(info_path, "init=" + init_path.filename().string())) {
            return init_path.filename();
        }



        return init_path.filename();
    }
} /* namespace Tools */