#pragma once

// builtin
#include <chrono>
#include <iostream>

// local
#include "../utils/logging/log.hpp"

struct TimeUnit
{
    std::string_view print_string;
    int conversion_factor;
};

class TimeMeasurer
{
public:

    inline static const TimeUnit SECOND{"s", 1000000};
    inline static const TimeUnit MILLISECOND{"ms", 1000};
    inline static const TimeUnit MICROSECOND{"us", 1};

private:

    std::string message;
    TimeUnit time_unit;

    std::chrono::system_clock::time_point start_time;

public:

    TimeMeasurer(std::string&& message = "", const TimeUnit time_unit = MILLISECOND);

    void restart();

    double get_time() const;

    void print_time() const;
};