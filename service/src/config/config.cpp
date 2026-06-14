#include "config.hpp"

#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

using Json = nlohmann::json;

Config ConfigLoader::load(const std::string& pathname) {
    std::ifstream file(pathname);

    if (!file.is_open()) {
        throw std::runtime_error("Falied to open config: " + pathname);
    }

    Json json;

    file >> json;

    Config config;

    const auto& api = json.at("api");
    const auto& region = json.at("region");
    const auto& led = json.at("led");

    config.api.url = api.at("url").get<std::string>();
    config.api.poll_interval_seconds = api.at("poll_interval_seconds").get<int>();

    config.region.name = region.at("name").get<std::string>();

    config.led.gpio_pin = led.at("gpio_pin").get<int>();
    config.led.active_alarm_blink_hz = led.at("active_alarm_blink_hz").get<double>();
    config.led.neighbor_alarm_blink_hz = led.at("neighbor_alarm_blink_hz").get<double>();

    validate(config);

    return config;
}

void ConfigLoader::validate(const Config& config) {
    if (config.api.url.empty()) {
        throw std::runtime_error("API URL is empty");
    }

    if (config.api.poll_interval_seconds <= 0) {
        throw std::runtime_error("Poll interval must be positive");
    }

    if (config.region.name.empty()) {
        throw std::runtime_error("Region must not be empty");
    }

    if (config.led.gpio_pin < 0) {
        throw std::runtime_error("Invalid GPIO");
    }

    if (config.led.active_alarm_blink_hz <= 0) {
        throw std::runtime_error("Active alarm blink frequency must be positive");
    }
    if (config.led.neighbor_alarm_blink_hz <= 0) {
        throw std::runtime_error("Neighbor alarm blink frequency must be positive");
    }
}