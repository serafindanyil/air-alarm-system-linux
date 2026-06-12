#include "app/application.hpp"

#include <csignal>

Application* global_application = nullptr;

void signal_handler(int) {
    if (global_application != nullptr) {
        global_application->stop();
    }
}

int main() {
    Application application;

    global_application = &application;

    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    return application.run();
}