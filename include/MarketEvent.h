#ifndef MARKET_EVENT_H
#define MARKET_EVENT_H

#include <string>
#include "Event.h"

class MarketEvent {
private:
    std::string timestamp;
    std::string symbol;
    double open;
    double high;
    double low;
    double close;
    long long volume;

public:
    MarketEvent(
        const std::string& timestamp,
        const std::string& symbol,
        double open,
        double high,
        double low,
        double close,
        long long volume
    );

    EventType getType() const;
    const std::string& getTimestamp() const;
    const std::string& getSymbol() const;
    double getOpen() const;
    double getHigh() const;
    double getLow() const;
    double getClose() const;
    long long getVolume() const;
};

#endif