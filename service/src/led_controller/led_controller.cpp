#include <chrono>
#include <stdexcept>

#include "led_controller.hpp"

LedController::LedController(Gpio& gpio) : gpio_(gpio) {
    gpio_.set(false);

    if (running_) {
        return;
    }

    running_ = true;
    worker_ = std::thread(&LedController::workerLoop, this);
}

LedController::~LedController() {
    if (!running_) {
        return;
    }

    running_ = false;
    condition_.notify_all();

    if (worker_.joinable()) {
        worker_.join();
    }

    led_state_ = false;
    gpio_.set(false);
}

void LedController::active() {
    {
        std::lock_guard lock(mutex_);
        mode_ = Mode::Active;
    }

    condition_.notify_all();
}

void LedController::disabled() {
    {
        std::lock_guard lock(mutex_);
        mode_ = Mode::Disabled;
    }

    condition_.notify_all();
}

void LedController::blink(double hz) {
    if (hz <= 0.0) {
        throw std::runtime_error("Blink frequency must be positive");
    }

    {
        std::lock_guard lock(mutex_);
        blink_hz_ = hz;
        mode_ = Mode::Blink;
    }

    condition_.notify_all();
}

void LedController::workerLoop() {
    while (running_) {
        Mode current_mode;
        double current_hz;

        {
            std::unique_lock lock(mutex_);
            current_mode = mode_;
            current_hz = blink_hz_;
        }

        if (current_mode == Mode::Disabled) {
            led_state_ = false;
            gpio_.set(false);

            std::unique_lock lock(mutex_);
            condition_.wait(lock, [this] { return !running_ || mode_ != Mode::Disabled; });
        }

        if (current_mode == Mode::Active) {
            led_state_ = true;
            gpio_.set(true);

            std::unique_lock lock(mutex_);
            condition_.wait(lock, [this] { return !running_ || mode_ != Mode::Active; });
        }

        if (current_mode == Mode::Blink) {
            led_state_ = !led_state_;
            gpio_.set(led_state_);

            const auto half_period = std::chrono::duration<double>(1.0 / (current_hz * 2.0));

            std::unique_lock lock(mutex_);
            condition_.wait_for(lock, half_period,
                                [this] { return !running_ || mode_ != Mode::Blink; });
        }
    }

    led_state_ = false;
    gpio_.set(false);
}