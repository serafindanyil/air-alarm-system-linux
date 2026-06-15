cmake_minimum_required(VERSION 3.20)

project(AirAlarmService VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(nlohmann_json 3 REQUIRED)
find_package(cpr REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GPIOD REQUIRED libgpiod)

add_executable(
    air-alarm-service
    src/main.cpp
    src/app/application.cpp
    src/config/config.cpp
    src/region_list/region_list.cpp
    src/api/api.cpp
    src/gpio/gpio.cpp
    src/led_controller/led_controller.cpp
)

target_link_libraries(
    air-alarm-service
    PRIVATE
        nlohmann_json::nlohmann_json
        cpr::cpr
        ${GPIOD_LIBRARIES}
)

target_include_directories(
    air-alarm-service
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${GPIOD_LIBRARIES}

)

target_compile_options(air-alarm-service
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)