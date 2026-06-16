#pragma once

#include <atomic>

class Application {
public:
    Application();

    int run();
    void stop();

    bool hasInternetConnection() const;

private:
    bool checkInternetConnection() const;

    int ping_poll_interval_seconds_ = 3;

    std::atomic<bool> running_{true};
    std::atomic<bool> has_internet_connection_{false};
};