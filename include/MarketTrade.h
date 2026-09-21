#ifndef MARKET_TRADE_H
#define MARKET_TRADE_H

#include <string>

class MarketTrade {
private:
    long long timestamp;
    std::string symbol;
    double price;
    int size;
    char side;
    long long sequence;

public:
    MarketTrade(
        long long timestamp,
        const std::string& symbol,
        double price,
        int size,
        char side,
        long long sequence
    );

    long long getTimestamp() const;
    const std::string& getSymbol() const;
    double getPrice() const;
    int getSize() const;
    char getSide() const;
    long long getSequence() const;
};

#endif