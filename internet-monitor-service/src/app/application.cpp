#include "application.hpp"
#include "logging/logging.hpp"

#include <chrono>
#include <cstdlib>
#include <thread>

Logging logger("APPLICATION");

Application::Application() {
    setupDbus();
}

void Application::setupDbus() {
    connection_ =
        sdbus::createSystemBusConnection(sdbus::ServiceName{"org.airalarm.InternetMonitor"});

    object_ = sdbus::createObject(*connection_, sdbus::ObjectPath{"/org/airalarm/InternetMonitor"});

    object_
        ->addVTable(sdbus::registerMethod("HasInternetConnection").implementedAs([this]() {
            return hasInternetConnection();
        }))
        .forInterface(sdbus::InterfaceName{"org.airalarm.InternetMonitor"});
}

int Application::run() {
    logger.print("Starting Internet Monitor...");

    connection_->enterEventLoopAsync();

    while (running_) {
        has_internet_connection_ = checkInternetConnection();

        if (has_internet_connection_) {
            logger.print("Internet connection is available");
        } else {
            logger.print("Internet connection is unavailable");
        }

        std::this_thread::sleep_for(std::chrono::seconds(PING_POLL_INTERVAL_SECONDS));
    }

    connection_->leaveEventLoop();

    logger.print("Stopping Internet Monitor...");

    return 0;
}

void Application::stop() {
    running_ = false;
}

bool Application::hasInternetConnection() const {
    return has_internet_connection_;
}

bool Application::checkInternetConnection() const {
    const int result = std::system("ping -c 1 -W 1 8.8.8.8 > /dev/null 2>&1");

    return result == 0;
}
