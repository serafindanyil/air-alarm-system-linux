#include "api/api.hpp"
#include "app/application.hpp"
#include "config/config.hpp"
#include "gpio/gpio.hpp"
#include "led_controller/led_controller.hpp"
#include "logging/logging.hpp"
#include "region_list/region_list.hpp"

#include <csignal>

Application* global_application = nullptr;

Logging runtime_error_logger("RUNTIME_ERROR");

void signal_handler(int) {
    if (global_application != nullptr) {
        global_application->stop();
    }
}

int main() {
    try {
        Config config = ConfigLoader::load("/etc/air-alarm-service/config.json");

        Gpio gpio = Gpio(config.led.gpio_chip_path, config.led.gpio_pin);
        LedController led_controller = LedController(gpio);

        RegionList region_list = RegionListLoader::load("config/regions_adjacency_list.json");

        const int region_id = RegionListLoader::getRegionId(region_list, config.region.name);

        Api api = Api(config.api.url, region_id, region_list);

        Application application(config, led_controller, api, runtime_error_logger);

        global_application = &application;

        std::signal(SIGINT, signal_handler);
        std::signal(SIGTERM, signal_handler);

        return application.run();
    } catch (const std::exception& exception) {
        runtime_error_logger.print(std::string(exception.what()));
        return 1;
    }
}