#include "MarketEvent.h"

MarketEvent::MarketEvent(
    const std::string& timestamp,
    const std::string& symbol,
    double open,
    double high,
    double low,
    double close,
    long long volume
)
    : timestamp(timestamp),
      symbol(symbol),
      open(open),
      high(high),
      low(low),
      close(close),
      volume(volume)
{
}

EventType MarketEvent::getType() const {
    return EventType::MARKET;
}

const std::string& MarketEvent::getTimestamp() const {
    return timestamp;
}

const std::string& MarketEvent::getSymbol() const {
    return symbol;
}

double MarketEvent::getOpen() const {
    return open;
}

double MarketEvent::getHigh() const {
    return high;
}

double MarketEvent::getLow() const {
    return low;
}

double MarketEvent::getClose() const {
    return close;
}

long long MarketEvent::getVolume() const {
    return volume;
}