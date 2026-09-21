#ifndef MARKET_BAR_H
#define MARKET_BAR_H

#include <string>

class MarketBar {
private:
    std::string timestamp;
    std::string symbol;

    double open;
    double high;
    double low;
    double close;

    long long volume;

public:
    MarketBar(
        const std::string& timestamp,
        const std::string& symbol,
        double open,
        double high,
        double low,
        double close,
        long long volume
    );

    const std::string& getTimestamp() const;
    const std::string& getSymbol() const;

    double getOpen() const;
    double getHigh() const;
    double getLow() const;
    double getClose() const;

    long long getVolume() const;
};

#endif