#include "app/application.hpp"

#include <csignal>

Application* global_application = nullptr;

void signalHandler(int) {
    if (global_application != nullptr) {
        global_application->stop();
    }
}

int main() {
    Application application;

    global_application = &application;

    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    return application.run();
}