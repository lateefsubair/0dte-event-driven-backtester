#include <iostream>
#include <memory>

#include "MarketEvent.h"
#include "EventQueue.h"

int main() {

    EventQueue eventQueue;

    auto event1 = std::make_shared<MarketEvent>(
        "2023-01-03 09:35:00",
        "SPY",
        384.37,
        385.12,
        384.20,
        384.91,
        1245600
    );

    auto event2 = std::make_shared<MarketEvent>(
        "2023-01-03 09:40:00",
        "SPY",
        384.91,
        385.40,
        384.70,
        385.25,
        987300
    );

    auto event3 = std::make_shared<MarketEvent>(
        "2023-01-03 09:45:00",
        "SPY",
        385.25,
        385.60,
        384.95,
        385.10,
        1104200
    );

    eventQueue.push(event1);
    eventQueue.push(event2);
    eventQueue.push(event3);

    std::cout << "Events in queue: "
              << eventQueue.size()
              << '\n';

    while (!eventQueue.empty()) {

        auto event = eventQueue.pop();

        std::cout << "\nProcessing MARKET event\n";
        std::cout << "Timestamp: " << event->getTimestamp() << '\n';
        std::cout << "Symbol: " << event->getSymbol() << '\n';
        std::cout << "Close: " << event->getClose() << '\n';
        std::cout << "Volume: " << event->getVolume() << '\n';
    }

    return 0;
}