//header
#include "./time_utils.hpp"

//third party
#include <chrono>
#include <fmt/format.h>
#include <string_view>

TimeMeasurer::TimeMeasurer(std::string&& message, const TimeUnit time_unit)
    :message{message}, time_unit{time_unit}, start_time{std::chrono::high_resolution_clock::now()}
{

}

void TimeMeasurer::restart()
{
    this->start_time = std::chrono::high_resolution_clock::now();
}

double TimeMeasurer::get_time() const
{   
    const auto current_time = std::chrono::high_resolution_clock::now();
    const auto elapsed_time = current_time - this->start_time;

    return static_cast<double>
        (std::chrono::duration_cast<std::chrono::microseconds>(elapsed_time).count()) / 
         this->time_unit.conversion_factor;

}

void TimeMeasurer::print_time() const
{
        const auto elapsed_time = this->get_time();
        const auto formated_message = fmt::format("{} executed in {}{}",
            this->message, elapsed_time,
            this->time_unit.print_string);

        notice(formated_message);
};