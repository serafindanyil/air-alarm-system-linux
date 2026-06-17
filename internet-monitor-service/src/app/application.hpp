#pragma once

#include <atomic>
#include <memory>

#include <sdbus-c++/sdbus-c++.h>

class Application {
public:
    Application();

    int run();
    void stop();

    bool hasInternetConnection() const;

private:
    void setupDbus();
    bool checkInternetConnection() const;

    int PING_POLL_INTERVAL_SECONDS = 3;

    std::atomic<bool> running_{true};
    std::atomic<bool> has_internet_connection_{false};

    std::unique_ptr<sdbus::IConnection> connection_;
    std::unique_ptr<sdbus::IObject> object_;
};