#include <cstdlib>
#include <cstdio>

#include "../include/arg_parse.hpp"
#include "../include/logger.hpp"
#include "../include/window.hpp"


auto
main(int32_t argc, char **argv) -> int32_t
{
    ArgParser arg_parse(argc, argv);

    if (arg_parse.find_arg({ "-h", "--help" })) {
        arg_parse.print_help_message(stdout);
    }

    Logger logger(&arg_parse);

    if (Utils::contains_substr(std::getenv("XDG_CURRENT_DESKTOP"), "hyprland")) {
        std::system("hyprctl keyword windowrule float,class:'^(control)$'");
    }

    argc = 0;
    auto app = Gtk::Application::create("org.void.better-control");
    BetterControl better_control(&logger, &arg_parse, "Control Centre", { 900, 600 });
    return app->run(better_control, argc, argv);
}