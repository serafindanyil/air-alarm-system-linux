cmake_minimum_required(VERSION 3.20)

project(InternetMonitorService VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(sdbus-c++ REQUIRED)


add_executable(
    internet-monitor-service
    src/main.cpp
    src/app/application.cpp
    src/logging/logging.cpp
)

target_link_libraries(
    internet-monitor-service
    PRIVATE
        SDBusCpp::sdbus-c++
)

target_include_directories(
    internet-monitor-service
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(
    internet-monitor-service
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)