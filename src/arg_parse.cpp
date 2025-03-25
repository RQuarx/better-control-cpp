#include "../inc/arg_parse.hpp"
#include "glibmm/thread.h"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <print>
#include <string_view>


ArgParser::ArgParser(int32_t argc, char **argv)
{
    arg_list = std::vector<std::string_view>(argv, std::next(argv, argc));
}


auto
ArgParser::find_arg(ArgInput arg) -> bool
{
    return (
        arg.long_arg.empty()
        ? std::ranges::find(arg_list, arg.short_arg) != arg_list.end()
        : std::ranges::find(arg_list, arg.short_arg) != arg_list.end()
            || std::ranges::find(arg_list, arg.long_arg) != arg_list.end()
    );
}


auto
ArgParser::option_arg(std::string &option, ArgInput arg) -> bool
{
    for (size_t i = 1; i < arg_list.size(); i++) {
        std::string_view current_arg = arg_list.at(i);
        size_t eq_index = current_arg.find('=');

        if (eq_index != std::string_view::npos) {
            current_arg = current_arg.substr(0, eq_index);
        }

        if (current_arg != arg.short_arg && current_arg != arg.long_arg) continue;

        if (eq_index != std::string_view::npos) {
            option = current_arg.substr(eq_index + 1);
            return true;
        }

        if (i + 1 < arg_list.size()) {
            std::string_view next_arg = arg_list.at(i + 1);
            if (!next_arg.starts_with('-')) {
                option = next_arg;
                return true;
            }
        }
    }
    return false;
}


void
ArgParser::print_help_message(FILE *stream)
{
    std::println(stream, "Usage: {} <options>\n", arg_list.at(0));
    std::println(stream, "Options:");
    exit(stream == stderr ? EXIT_FAILURE : EXIT_SUCCESS);
}