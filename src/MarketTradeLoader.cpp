#include "MarketTradeLoader.h"

#include <fstream>
#include <string>
#include <sstream>

std::vector<MarketTrade>
MarketTradeLoader::loadCSV(
    const std::string& filePath
) {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        return {};
    }

    std::vector<MarketTrade> trades;

    std::string line;

    // Skip the CSV header.
    std::getline(file, line);

    while (std::getline(file, line)) {

        // 1. Skip completely empty rows.
        if (line.empty()) {
            continue;
        }

        std::stringstream ss(line);

        std::string timestampString;
        std::string symbol;
        std::string priceString;
        std::string sizeString;
        std::string sideString;
        std::string sequenceString;

        // 2. Skip rows that do not contain all 6 fields.
        if (!std::getline(ss, timestampString, ',') ||
            !std::getline(ss, symbol, ',') ||
            !std::getline(ss, priceString, ',') ||
            !std::getline(ss, sizeString, ',') ||
            !std::getline(ss, sideString, ',') ||
            !std::getline(ss, sequenceString, ',')) {

            continue;
            }


        try {
            long long timestamp = std::stoll(timestampString);
            double price = std::stod(priceString);
            int size = std::stoi(sizeString);
            char side = sideString.empty() ? ' ' : sideString[0];
            long long sequence = std::stoll(sequenceString);

            MarketTrade trade(
                timestamp,
                symbol,
                price,
                size,
                side,
                sequence
            );

            trades.push_back(trade);
        }
        catch (const std::exception&) {
            continue;
        }
    }

    return trades;
}