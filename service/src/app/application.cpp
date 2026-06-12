#include "application.hpp"

#include <iostream>

Application::Application() = default;

int Application::run() {
    std::cout << "Air Alarm Service started" << std::endl;

    return 0;
}