#include "MarketBar.h"

MarketBar::MarketBar(
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
      volume(volume) {
}

const std::string& MarketBar::getTimestamp() const {
    return timestamp;
}

const std::string& MarketBar::getSymbol() const {
    return symbol;
}

double MarketBar::getOpen() const {
    return open;
}

double MarketBar::getHigh() const {
    return high;
}

double MarketBar::getLow() const {
    return low;
}

double MarketBar::getClose() const {
    return close;
}

long long MarketBar::getVolume() const {
    return volume;
}