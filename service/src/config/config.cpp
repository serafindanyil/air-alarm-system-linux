#include "config.hpp"
#include <logging/logging.hpp>

#include <fstream>

#include <nlohmann/json.hpp>

using Json = nlohmann::json;

Logging config_error_logger("CONFIG");

Config ConfigLoader::load(const std::string& pathname) {
    std::ifstream file(pathname);

    if (!file.is_open()) {
        config_error_logger.print("Failed to open config: " + pathname);
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
    config.led.gpio_chip_path = led.at("gpio_chip_path").get<std::string>();
    config.led.active_alarm_blink_hz = led.at("active_alarm_blink_hz").get<double>();
    config.led.neighbor_alarm_blink_hz = led.at("neighbor_alarm_blink_hz").get<double>();

    validate(config);

    return config;
}

void ConfigLoader::validate(const Config& config) {
    const auto& api = config.api;
    const auto& region = config.region;
    const auto& led = config.led;

    if (api.url.empty()) {
        config_error_logger.print("API URL is empty");
    }

    if (!api.url.starts_with("http://") && !api.url.starts_with("https://")) {
        config_error_logger.print("API URL must start with http:// or https://");
    }

    if (api.poll_interval_seconds < 3) {
        config_error_logger.print("Poll interval must be at least 3 seconds");
    }

    if (region.name.empty()) {
        config_error_logger.print("Region must not be empty");
    }

    if (led.gpio_pin < 0) {
        config_error_logger.print("Invalid GPIO");
    }

    if (led.gpio_chip_path.empty()) {
        config_error_logger.print("GPIO chip path is empty");
    }

    if (led.active_alarm_blink_hz <= 0) {
        config_error_logger.print("Active alarm blink frequency must be positive");
    }
    if (led.neighbor_alarm_blink_hz <= 0) {
        config_error_logger.print("Neighbor alarm blink frequency must be positive");
    }
}