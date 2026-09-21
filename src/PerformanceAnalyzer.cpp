#include "PerformanceAnalyzer.h"
#include <cmath>

std::vector<double> PerformanceAnalyzer::calculateReturns(
    const std::vector<double>& equityCurve
) {
    std::vector<double> returns;

    if (equityCurve.size() < 2) {
        return returns;
    }

    for (std::size_t i = 1; i < equityCurve.size(); ++i) {

        double previousEquity = equityCurve[i - 1];
        double currentEquity = equityCurve[i];

        double returnValue =
            (currentEquity / previousEquity) - 1.0;

        returns.push_back(returnValue);
    }

    return returns;
}

double PerformanceAnalyzer::calculateAverageReturn(
    const std::vector<double>& returns
) {
    if (returns.empty()) {
        return 0.0;
    }

    double sum = 0.0;

    for (double returnValue : returns) {
        sum += returnValue;
    }

    return sum / returns.size();
}

double PerformanceAnalyzer::calculateVolatility(
    const std::vector<double>& returns
) {
    if (returns.size() < 2) {
        return 0.0;
    }

    double averageReturn =
        calculateAverageReturn(returns);

    double squaredDifferenceSum = 0.0;

    for (double returnValue : returns) {

        double difference =
            returnValue - averageReturn;

        squaredDifferenceSum +=
            difference * difference;
    }

    double variance =
        squaredDifferenceSum /
        (returns.size() - 1);

    return std::sqrt(variance);
}

double PerformanceAnalyzer::calculateSharpeRatio(
    const std::vector<double>& returns
) {
    if (returns.size() < 2) {
        return 0.0;
    }

    double averageReturn =
        calculateAverageReturn(returns);

    double volatility =
        calculateVolatility(returns);

    if (volatility == 0.0) {
        return 0.0;
    }

    return averageReturn / volatility;
}

double PerformanceAnalyzer::calculateMaxDrawdown(
    const std::vector<double>& equityCurve
) {
    if (equityCurve.empty()) {
        return 0.0;
    }

    double peak = equityCurve[0];
    double maxDrawdown = 0.0;

    for (double equity : equityCurve) {

        if (equity > peak) {
            peak = equity;
        }

        double drawdown =
            (peak - equity) / peak;

        if (drawdown > maxDrawdown) {
            maxDrawdown = drawdown;
        }
    }

    return maxDrawdown;
}

double PerformanceAnalyzer::calculateTotalReturn(
    const std::vector<double>& equityCurve
) {
    if (equityCurve.size() < 2) {
        return 0.0;
    }

    double initialEquity = equityCurve.front();
    double finalEquity = equityCurve.back();

    return (finalEquity / initialEquity) - 1.0;
}

double PerformanceAnalyzer::calculateWinRate(
    const std::vector<double>& returns
) {
    if (returns.empty()) {
        return 0.0;
    }

    int winningPeriods = 0;

    for (double returnValue : returns) {
        if (returnValue > 0.0) {
            winningPeriods++;
        }
    }

    return static_cast<double>(winningPeriods) /
           returns.size();
}

double PerformanceAnalyzer::calculateTradeWinRate(
    const std::vector<Trade>& trades
) {
    if (trades.empty()) {
        return 0.0;
    }

    int winningTrades = 0;

    for (const Trade& trade : trades) {
        if (trade.getNetPnL() > 0.0) {
            winningTrades++;
        }
    }

    return static_cast<double>(winningTrades) /
           trades.size();
}

std::size_t PerformanceAnalyzer::calculateTradeCount(
    const std::vector<Trade>& trades
) {
    return trades.size();
}

double PerformanceAnalyzer::calculateAverageTradePnL(
    const std::vector<Trade>& trades
) {
    if (trades.empty()) {
        return 0.0;
    }

    double totalPnL = 0.0;

    for (const Trade& trade : trades) {
        totalPnL += trade.getNetPnL();
    }

    return totalPnL / trades.size();
}

double PerformanceAnalyzer::calculateProfitFactor(
    const std::vector<Trade>& trades
) {
    if (trades.empty()) {
        return 0.0;
    }

    double grossProfit = 0.0;
    double grossLoss = 0.0;

    for (const Trade& trade : trades) {

        double netPnL = trade.getNetPnL();

        if (netPnL > 0.0) {
            grossProfit += netPnL;
        }

        if (netPnL < 0.0) {
            grossLoss += -netPnL;
        }
    }

    if (grossLoss == 0.0) {
        return 0.0;
    }

    return grossProfit / grossLoss;
}

double PerformanceAnalyzer::calculateTotalNetPnL(
    const std::vector<Trade>& trades
) {
    double totalNetPnL = 0.0;

    for (const Trade& trade : trades) {
        totalNetPnL += trade.getNetPnL();
    }

    return totalNetPnL;
}

double PerformanceAnalyzer::calculateAverageWinningTrade(
    const std::vector<Trade>& trades
) {
    double totalWinningPnL = 0.0;
    int winningTrades = 0;

    for (const Trade& trade : trades) {
        double netPnL = trade.getNetPnL();

        if (netPnL > 0.0) {
            totalWinningPnL += netPnL;
            winningTrades++;
        }
    }

    if (winningTrades == 0) {
        return 0.0;
    }

    return totalWinningPnL / winningTrades;
}

double PerformanceAnalyzer::calculateAverageLosingTrade(
    const std::vector<Trade>& trades
) {
    double totalLosingPnL = 0.0;
    int losingTrades = 0;

    for (const Trade& trade : trades) {
        double netPnL = trade.getNetPnL();

        if (netPnL < 0.0) {
            totalLosingPnL += -netPnL;
            losingTrades++;
        }
    }

    if (losingTrades == 0) {
        return 0.0;
    }

    return totalLosingPnL / losingTrades;
}