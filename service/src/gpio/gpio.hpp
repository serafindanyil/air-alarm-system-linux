#pragma once

#include <gpiod.h>

#include <string>

class Gpio {
public:
    Gpio(const std::string& chip_name, unsigned int pin);
    ~Gpio();

    Gpio(const Gpio&) = delete;
    Gpio& operator=(const Gpio&) = delete;

    void set(bool value);

private:
    gpiod_chip* chip_ = nullptr;
    gpiod_line_request* request_ = nullptr;

    unsigned int pin_;
};