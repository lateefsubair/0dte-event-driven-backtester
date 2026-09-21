#ifndef MARKET_DATA_LOADER_H
#define MARKET_DATA_LOADER_H

#include <string>
#include <vector>
#include <memory>

#include "MarketEvent.h"

class MarketDataLoader {
public:
    static std::vector<std::shared_ptr<MarketEvent>> loadCSV(
        const std::string& filePath
    );
};

#endif