
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include "MarketDataLoader.h"

std::vector<std::shared_ptr<MarketEvent>>
MarketDataLoader::loadCSV(
    const std::string& filePath
) {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open market data file: "
                  << filePath
                  << '\n';

        return {};
    }

    std::string line;

    std::vector<std::shared_ptr<MarketEvent>> marketEvents;

    // Skip the CSV header.
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        std::stringstream ss(line);

        std::string timestamp;
        std::string symbol;
        std::string openString;
        std::string highString;
        std::string lowString;
        std::string closeString;
        std::string volumeString;

        if (!std::getline(ss, timestamp, ',') ||
            !std::getline(ss, symbol, ',') ||
            !std::getline(ss, openString, ',') ||
            !std::getline(ss, highString, ',') ||
            !std::getline(ss, lowString, ',') ||
            !std::getline(ss, closeString, ',') ||
            !std::getline(ss, volumeString, ',')) {

            continue;
            }

        try {
            double open = std::stod(openString);
            double high = std::stod(highString);
            double low = std::stod(lowString);
            double close = std::stod(closeString);

            long long volume = std::stoll(volumeString);

            if (open <= 0.0 ||
            high <= 0.0 ||
            low <= 0.0 ||
            close <= 0.0 ||
            volume < 0) {

                continue;
    }
            // Validate OHLC consistency.
            if (high < open ||
                high < close ||
                low > open ||
                low > close ||
                high < low) {

                continue;
                }


            auto marketEvent = std::make_shared<MarketEvent>(
                timestamp,
                symbol,
                open,
                high,
                low,
                close,
                volume
            );

            marketEvents.push_back(marketEvent);
        }
        catch (const std::exception&) {
            continue;
        }
    }

    return marketEvents;
}