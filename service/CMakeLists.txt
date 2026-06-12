cmake_minimum_required(VERSION 3.20)

project(AirAlarmService VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(nlohmann_json 3 REQUIRED)

add_executable(
    air-alarm-service
    src/main.cpp
    src/app/application.cpp
    src/config/config.cpp
)

target_link_libraries(
    air-alarm-service
    PRIVATE
        nlohmann_json::nlohmann_json
)

target_include_directories(
    air-alarm-service
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(air-alarm-service
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)