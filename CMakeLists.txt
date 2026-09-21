cmake_minimum_required(VERSION 3.20)

project(project2_event_driven_backtester)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(project2_event_driven_backtester
        src/main.cpp
        include/MarketEvent.h
        src/MarketEvent.cpp
        include/EventQueue.h
        src/EventQueue.cpp
)

target_include_directories(project2_event_driven_backtester PRIVATE include)