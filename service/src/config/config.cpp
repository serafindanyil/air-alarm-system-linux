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

    config.api.url = json.at("api").at("url").get<std::string>();
    config.api.poll_interval_seconds = json.at("api").at("poll_interval_seconds").get<int>();

    config.region.name = json.at("region").at("name").get<std::string>();

    config.led.gpio_pin = json.at("led").at("gpio_pin").get<int>();
    config.led.active_alarm_blink_hz = json.at("led").at("active_alarm_blink_hz").get<double>();
    config.led.neighbor_alarm_blink_hz = json.at("led").at("neighbor_alarm_blink_hz").get<double>();

    if (config.api.url.empty()) {
        throw std::runtime_error("API URL is empty");
    }

    if (!config.api.url.starts_with("https://")) {
        throw std::runtime_error("Invalid API URL");
    }

    if (config.api.poll_interval_seconds <= 0) {
        throw std::runtime_error("Poll interval must be positive");
    }

    if (config.region.name.empty()) {
        throw std::runtime_error("Region must not be empty");
    }

    if (config.led.gpio_pin <= 0) {
        throw std::runtime_error("Invalid GPIO");
    }

    if (config.led.active_alarm_blink_hz <= 0) {
        throw std::runtime_error("Active alarm blink frequency must be positive");
    }
    if (config.led.neighbor_alarm_blink_hz <= 0) {
        throw std::runtime_error("Neighbor alarm blink frequency must be positive");
    }

    return config;
}