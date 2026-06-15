#include "gpio/gpio.hpp"

#include <stdexcept>

Gpio::Gpio(const std::string& chip_name, unsigned int pin) : pin_(pin) {
    chip_ = gpiod_chip_open(chip_name.c_str());

    if (chip_ == nullptr) {
        throw std::runtime_error("Failed to open GPIO");
    }

    gpiod_line_settings* settings = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
    gpiod_line_settings_set_output_value(settings, GPIOD_LINE_VALUE_INACTIVE);

    gpiod_line_config* line_config = gpiod_line_config_new();

    unsigned int offsets[] = {pin_};

    gpiod_line_config_add_line_settings(line_config, offsets, 1, settings);

    gpiod_request_config* request_config = gpiod_request_config_new();
    gpiod_request_config_set_consumer(request_config, "air-alarm-service");

    request_ = gpiod_chip_request_lines(chip_, request_config, line_config);

    gpiod_request_config_free(request_config);
    gpiod_line_config_free(line_config);
    gpiod_line_settings_free(settings);

    if (request_ == nullptr) {
        gpiod_chip_close(chip_);
        throw std::runtime_error("Failed to request GPIO line");
    }
}

Gpio::~Gpio() {
    if (request_ != nullptr) {
        gpiod_line_request_release(request_);
    }

    if (chip_ != nullptr) {
        gpiod_chip_close(chip_);
    }
}

void Gpio::set(bool value) {
    gpiod_line_request_set_value(request_, pin_,
                                 value ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE);
}