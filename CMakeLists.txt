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
        include/SignalEvent.h
        src/SignalEvent.cpp
        include/OrderEvent.h
        src/OrderEvent.cpp
        include/FillEvent.h
        src/FillEvent.cpp
        include/Strategy.h
        src/Strategy.cpp
        include/Portfolio.h
        src/Portfolio.cpp
        include/ExecutionHandler.h
        src/ExecutionHandler.cpp
        include/BacktestEngine.h
        src/BacktestEngine.cpp
        include/PerformanceAnalyzer.h
        src/PerformanceAnalyzer.cpp
        include/Trade.h
        src/Trade.cpp
        include/MarketDataLoader.h
        src/MarketDataLoader.cpp
        include/MarketBar.h
        src/MarketBar.cpp
        include/MarketTrade.h
        src/MarketTrade.cpp
        include/MarketTradeLoader.h
        src/MarketTradeLoader.cpp
        include/MarketBarAggregator.h
        src/MarketBarAggregator.cpp
        include/TimestampUtils.h
        src/TimestampUtils.cpp
)

target_include_directories(project2_event_driven_backtester PRIVATE include)