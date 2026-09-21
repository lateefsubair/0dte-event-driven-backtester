#include "MarketTrade.h"

MarketTrade::MarketTrade(
    long long timestamp,
    const std::string& symbol,
    double price,
    int size,
    char side,
    long long sequence
)
    : timestamp(timestamp),
      symbol(symbol),
      price(price),
      size(size),
      side(side),
      sequence(sequence) {
}

long long MarketTrade::getTimestamp() const {
    return timestamp;
}

const std::string& MarketTrade::getSymbol() const {
    return symbol;
}

double MarketTrade::getPrice() const {
    return price;
}

int MarketTrade::getSize() const {
    return size;
}

char MarketTrade::getSide() const {
    return side;
}

long long MarketTrade::getSequence() const {
    return sequence;
}