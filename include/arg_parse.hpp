#pragma once
#ifndef ARG_PARSE_
#define ARG_PARSE_

#include <string_view>
#include <cstdint>
#include <vector>
#include <array>

#define HELP_MSG \
"Help Options:\n" \
"  -h, --help                 Show help options\n" \
"  -l, --log <lvl/file>       A log option that accepts a path to a log file or a log-level (0-3)\n"


class
ArgParser
{
public:
    ArgParser(int32_t argc, char **argv);

    /// Searches for short and long arg in the arg list
    /// @param arg a struct containing the short and long form of the arg
    /// @return true if a match are found, or false if not
    auto find_arg(std::pair<std::string_view, std::string_view> arg) -> bool;

    /// Searches for and return short and long arg in the arg list
    /// @param option the option param that will be filled
    /// @param arg a struct containing both the short and long form of the arg
    /// @return true if a match are found, or false if not
    auto option_arg(std::string &option, std::pair<std::string_view, std::string_view> arg) -> bool;

    /// Prints the help message to the specified output stream
    /// @warning this function will exit the program with code EXIT_SUCCESS
    void print_help_message(FILE *stream);

private:
    /// An array of 2 (short, long) containing a vector of a pair,
    //  with a bool which dictated if the string_view is an option or not
    std::array<std::vector<std::pair<bool, std::string_view>>, 2> arg_list{};
    std::string_view bin_path;

    auto find_option_short(std::string &option, std::string_view short_arg) -> bool;
    auto find_option_long(std::string &option, std::string_view long_arg) -> bool;
};


#endif /* arg_parse.hpp */