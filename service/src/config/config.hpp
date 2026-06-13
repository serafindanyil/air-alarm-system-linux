#pragma once

#include <string>

struct ApiConfig {
    std::string url;
    int poll_interval_seconds;
};

struct RegionConfig {
    std::string name;
};

struct LedConfig {
    int gpio_pin;
    double active_alarm_blink_hz;
    double neighbor_alarm_blink_hz;
};

struct Config {
    ApiConfig api;
    RegionConfig region;
    LedConfig led;
};

class ConfigLoader {
public:
    static Config load(const std::string& pathname);

private:
    static void validate(const Config& config);
};