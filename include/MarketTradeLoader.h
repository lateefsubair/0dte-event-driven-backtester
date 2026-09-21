#ifndef MARKET_TRADE_LOADER_H
#define MARKET_TRADE_LOADER_H

#include <string>
#include <vector>

#include "MarketTrade.h"

class MarketTradeLoader {
public:
    static std::vector<MarketTrade> loadCSV(
        const std::string& filePath
    );
};

#endif