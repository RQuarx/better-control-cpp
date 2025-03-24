#include "../inc/arg_parse.hpp"

#include <cstdlib>
#include <string>
#include <print>


ArgParser::ArgParser(int32_t argc, char **argv)
{
    arg_list = std::vector<std::string_view>(std::next(argv, 1), std::next(argv, argc));
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
    auto iter = std::ranges::find(this->arg_list, arg.short_arg);
    auto iter_one = iter++;

    if (iter != this->arg_list.end() && iter_one != this->arg_list.end()) {
        option = *iter;
        this->arg_list.erase(iter);
        return true;
    }

    if (!arg.long_arg.empty()) {
        iter = std::ranges::find(this->arg_list, arg.long_arg);
        iter_one = iter++;

        if (iter != this->arg_list.end() && iter_one != this->arg_list.end()) {
            this->arg_list.erase(iter);
            option = *iter;
            return true;
        }
    }

    option.clear();
    return false;
}


void
ArgParser::print_help_message(FILE *stream)
{
    std::println(stream, "Usage: {} <options>\n", arg_list.at(0));
    std::println(stream, "Options:");
    exit(EXIT_SUCCESS);
}