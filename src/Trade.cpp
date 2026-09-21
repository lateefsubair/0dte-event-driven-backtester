#include "Trade.h"

Trade::Trade(
    const std::string& symbol,
    const std::string& entryTimestamp,
    const std::string& exitTimestamp,
    double entryPrice,
    double exitPrice,
    int quantity,
    double entryCommission,
    double exitCommission
)
    : symbol(symbol),
      entryTimestamp(entryTimestamp),
      exitTimestamp(exitTimestamp),
      entryPrice(entryPrice),
      exitPrice(exitPrice),
      quantity(quantity),
      entryCommission(entryCommission),
      exitCommission(exitCommission) {
}

const std::string& Trade::getSymbol() const {
    return symbol;
}

const std::string& Trade::getEntryTimestamp() const {
    return entryTimestamp;
}

const std::string& Trade::getExitTimestamp() const {
    return exitTimestamp;
}

double Trade::getEntryPrice() const {
    return entryPrice;
}

double Trade::getExitPrice() const {
    return exitPrice;
}

int Trade::getQuantity() const {
    return quantity;
}

double Trade::getEntryCommission() const {
    return entryCommission;
}

double Trade::getExitCommission() const {
    return exitCommission;
}

double Trade::getGrossPnL() const {
    return (exitPrice - entryPrice) * quantity;
}

double Trade::getNetPnL() const {
    return getGrossPnL()
           - entryCommission
           - exitCommission;
}

double Trade::getReturn() const {
    double entryValue = entryPrice * quantity;

    if (entryValue == 0.0) {
        return 0.0;
    }

    return getNetPnL() / entryValue;
}