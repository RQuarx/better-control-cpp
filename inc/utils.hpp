#pragma once
#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <string>


namespace Utils {
    static const int32_t MICROSECOND_UPPER_BOUND = 1000000;
    static const int32_t MICROSECOND_TO_MILISECOND = 1000;

    auto Get_Current_Time() -> std::string;
}

#endif /* utils.hpp */