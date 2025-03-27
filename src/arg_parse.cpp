#include "../include/arg_parse.hpp"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <print>
#include <string_view>


ArgParser::ArgParser(int32_t argc, char **argv)
{
    auto args = std::span(argv, argc);
    bin_path = args.front();

    uint8_t previous_type = 0;
    for (int32_t i = 1; i < argc; i++) {
        std::string_view arg = args[i];

        if (arg.starts_with("--")) {
            arg_list.at(1).emplace_back(false, arg.substr(2));
            previous_type = 1;
            continue;
        }

        if (arg.starts_with('-')) {
            arg_list.at(0).emplace_back(false, arg.substr(1));
            previous_type = 0;
            continue;
        }

        /* ? If an arg reaches this statement,
           ? that means the arg doesnt have a prefix of '-'.
           ? which means that it is an option of the previous type's arg
        */
        arg_list.at(previous_type).emplace_back(true, arg);
    }
}


auto
ArgParser::find_arg(std::pair<std::string_view, std::string_view> arg) -> bool
{
    /* ? Check for short arg and long arg */
    for (uint8_t i = 0, j = 1; i < 2; i++, j++) {
        if (
            std::ranges::any_of(
                arg_list.at(i),
                [&arg, i, j](const std::pair<bool, std::string_view> &a) -> bool
                { return !a.first && a.second == ( !i ? arg.first : arg.second ).substr(j); }
            )
        ) return true;
    }
    return false;
}


auto
ArgParser::option_arg(std::string &option, std::pair<std::string_view, std::string_view> arg) -> bool
{
    return (
        find_option_short(option, arg.first)
        || find_option_long(option, arg.second)
    );
}


void
ArgParser::print_help_message(FILE *stream)
{
    std::println(stream, "Usage: {} <options>\n", bin_path);
    std::println(stream, "Options:");
    exit(stream == stderr ? EXIT_FAILURE : EXIT_SUCCESS);
}


auto
ArgParser::find_option_short(std::string &option, std::string_view short_arg) -> bool
{
    for (size_t i = 0; i < arg_list.at(0).size(); i++) {
        const auto &a = arg_list.at(0).at(i);
        if (a.first) continue;

        if (a.second.contains('=')) {
            size_t eq_pos = a.second.find_first_of('=');

            std::pair<std::string_view, std::string_view> split_arg = {
                a.second.substr(0, eq_pos), a.second.substr(eq_pos + 1)
            };

            /* ? Check for -lo=a and -o=a */
            if (split_arg.first.back() == short_arg.at(1)) {
                option = split_arg.second;
                return true;
            }
        }

        /* ? Checks for -oa */
        if (a.second.length() > 1) {
            if (a.second.at(0) == short_arg.at(1)) {
                option = a.second.substr(1);
                return true;
            }
        }

        /* ? Checks for -lo a and -o a */
        for (const auto &c : a.second) {
            if (c == short_arg.at(1) && i + 1 < arg_list.at(0).size()) {
                const auto &next_arg = arg_list.at(0).at(i + 1);
                if (next_arg.first) {
                    option = next_arg.second;
                    return true;
                }
            }
        }
    }

    option.clear();
    return false;
}


auto
ArgParser::find_option_long(std::string &option, std::string_view long_arg) -> bool
{
    for (size_t i = 0; i < arg_list.at(1).size(); i++) {
        const auto &a = arg_list.at(1).at(i);
        if (a.first) continue;

        if (a.second.contains('=')) {
            size_t eq_pos = a.second.find_first_of('=');

            std::pair<std::string_view, std::string_view> split_arg = {
                a.second.substr(0, eq_pos), a.second.substr(eq_pos + 1)
            };

            /* ? Check for --option=a */
            if (split_arg.first == long_arg.substr(2)) {
                option = split_arg.second;
                return true;
            }
        }

        if (a.second == long_arg.substr(2) && i + 1 < arg_list.at(1).size()) {
            const auto &next_arg = arg_list.at(1).at(i + 1);

            if (next_arg.first) {
                option = next_arg.second;
                return true;
            }
        }
    }

    option.clear();
    return false;
}