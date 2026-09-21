#ifndef PERFORMANCE_ANALYZER_H
#define PERFORMANCE_ANALYZER_H

#include <vector>

#include "Trade.h"

class PerformanceAnalyzer {
public:
    static std::vector<double> calculateReturns(
        const std::vector<double>& equityCurve
    );

    static double calculateAverageReturn(
        const std::vector<double>& returns
    );

    static double calculateVolatility(
        const std::vector<double>& returns
    );

    static double calculateSharpeRatio(
        const std::vector<double>& returns
    );

    static double calculateMaxDrawdown(
    const std::vector<double>& equityCurve
    );

    static double calculateTotalReturn(
    const std::vector<double>& equityCurve
    );

    static double calculateWinRate(
    const std::vector<double>& returns
    );

    static double calculateTradeWinRate(
    const std::vector<Trade>& trades
    );

    static std::size_t calculateTradeCount(
    const std::vector<Trade>& trades
);

    static double calculateAverageTradePnL(
    const std::vector<Trade>& trades
);

    static double calculateProfitFactor(
    const std::vector<Trade>& trades
);

    static double calculateTotalNetPnL(
    const std::vector<Trade>& trades
);

    static double calculateAverageWinningTrade(
    const std::vector<Trade>& trades
);

    static double calculateAverageLosingTrade(
    const std::vector<Trade>& trades
);

};

#endif