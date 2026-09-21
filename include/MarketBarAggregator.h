#ifndef MARKET_BAR_AGGREGATOR_H
#define MARKET_BAR_AGGREGATOR_H

#include <vector>

#include "MarketTrade.h"
#include "MarketBar.h"

class MarketBarAggregator {
public:
    static std::vector<MarketBar> aggregateFiveMinuteBars(
        const std::vector<MarketTrade>& trades
    );
};

#endif