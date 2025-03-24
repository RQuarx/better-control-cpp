#pragma once
#ifndef ARG_PARSE_
#define ARG_PARSE_

#include <string_view>
#include <cstdint>
#include <vector>


struct
ArgInput
{
    std::string_view short_arg;
    std::string_view long_arg;
};


class
ArgParser
{
public:
    ArgParser(int32_t argc, char **argv);

    /// Searches for short and long arg in the arg list
    /// @param arg a struct containing the short and long form of the arg
    /// @return true if a match are found, or false if not
    auto find_arg(ArgInput arg) -> bool;

    /// Searches for and return short and long arg in the arg list
    /// @param option the option param that will be filled
    /// @param arg a struct containing both the short and long form of the arg
    /// @return true if a match are found, or false if not
    auto option_arg(std::string &option, ArgInput arg) -> bool;

    /// Prints the help message to the specified output stream
    /// @warning this function will exit the program with code EXIT_SUCCESS
    void print_help_message(FILE *stream);

private:
    std::vector<std::string_view> arg_list;
};


#endif /* arg_parse.hpp */