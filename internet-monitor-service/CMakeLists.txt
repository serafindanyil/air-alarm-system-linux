cmake_minimum_required(VERSION 3.20)

project(InternetMonitor VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_executable(
    internet-monitor
    src/main.cpp
    src/app/application.cpp
    src/logging/logging.cpp
)

target_include_directories(
    internet-monitor
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(
    internet-monitor
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)