#pragma once

#include <atomic>

class Application {
public:
    int run();
    void stop();

private:
    std::atomic<bool> running_{true};
};