#ifndef TRADE_H
#define TRADE_H

#include <string>

class Trade {
private:
    std::string symbol;
    std::string entryTimestamp;
    std::string exitTimestamp;

    double entryPrice;
    double exitPrice;

    int quantity;

    double entryCommission;
    double exitCommission;

public:
    Trade(
        const std::string& symbol,
        const std::string& entryTimestamp,
        const std::string& exitTimestamp,
        double entryPrice,
        double exitPrice,
        int quantity,
        double entryCommission,
        double exitCommission
    );

    const std::string& getSymbol() const;
    const std::string& getEntryTimestamp() const;
    const std::string& getExitTimestamp() const;

    double getEntryPrice() const;
    double getExitPrice() const;

    int getQuantity() const;

    double getEntryCommission() const;
    double getExitCommission() const;

    double getGrossPnL() const;
    double getNetPnL() const;
    double getReturn() const;
};

#endif