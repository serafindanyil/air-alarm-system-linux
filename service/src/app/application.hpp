#pragma once

#include <atomic>

class Application {
public:
    Application();

    int run();
    void stop();

private:
    std::atomic<bool> running_;
};