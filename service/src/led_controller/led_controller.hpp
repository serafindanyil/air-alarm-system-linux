#pragma once

#include "gpio/gpio.hpp"

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

class LedController {
public:
    explicit LedController(Gpio& gpio);
    ~LedController();

    void active();
    void disabled();
    void blink(double hz);

private:
    enum class Mode { Disabled, Active, Blink };

    void workerLoop();

    Gpio& gpio_;

    std::atomic<bool> running_{false};

    std::thread worker_;
    std::mutex mutex_;
    std::condition_variable condition_;

    bool led_state_{false};

    Mode mode_{Mode::Disabled};
    double blink_hz_{1.0};
};