#include "../inc/arg_parse.hpp"

#include <cstdlib>
#include <cstdio>


auto
main(int32_t argc, char **argv) -> int32_t
{
    ArgParser arg_parse(argc, argv);

    if (arg_parse.find_arg({ "-h", "--help" })) {
        arg_parse.print_help_message(stdout);
    }
}