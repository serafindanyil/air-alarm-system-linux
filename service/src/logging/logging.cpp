#include "logging.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

Logging::Logging(const std::string& state) : state_(state) {}

void Logging::print(const std::string& message) const {
    std::cout << "[Air Alarm Service]"
              << "[" << state_ << "]"
              << "[" << getTimestamp() << "] " << message << std::endl;
}

std::string Logging::getTimestamp() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t time = std::chrono::system_clock::to_time_t(now);

    std::tm local_time{};

    localtime_r(&time, &local_time);

    std::stringstream stream;

    stream << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");

    return stream.str();
}