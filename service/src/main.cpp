#include "app/application.hpp"
#include "config/config.hpp"

#include <csignal>

Application* global_application = nullptr;

void signal_handler(int) {
    if (global_application != nullptr) {
        global_application->stop();
    }
}

int main() {
    Config config = ConfigLoader::load("config/air_alarm_config.json");

    Application application(config);

    global_application = &application;

    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    return application.run();
}