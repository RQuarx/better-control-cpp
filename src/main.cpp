#include <cstdlib>
#include <cstdio>

#include "../inc/arg_parse.hpp"
#include "../inc/logger.hpp"
#include "../inc/window.hpp"
#include "../inc/tools.hpp"


auto
main(int32_t argc, char **argv) -> int32_t
{
    ArgParser arg_parse(argc, argv);

    if (arg_parse.find_arg({ "-h", "--help" })) {
        arg_parse.print_help_message(stdout);
    }

    std::string option;
    if (arg_parse.option_arg(option, { "-o", "--option" })) {
        std::println("{}", option);
    }

    Logger logger(&arg_parse);

    // auto app = Gtk::Application::create(argc, argv, "org.void.better-control");
    // BetterControl better_control("Control Centre", { 900, 600 });
    // return app->run(better_control);
}