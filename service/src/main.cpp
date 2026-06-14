#include "app/application.hpp"
#include "config/config.hpp"
#include "region_list/region_list.hpp"

#include <csignal>

Application* global_application = nullptr;

void signal_handler(int) {
    if (global_application != nullptr) {
        global_application->stop();
    }
}

int main() {
    Config config = ConfigLoader::load("config/air_alarm_config.json");

    RegionList region_list = RegionListLoader::load("config/regions_adjacency_list.json");

    Application application(config, region_list);

    global_application = &application;

    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    return application.run();
}