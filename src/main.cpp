#include <iostream>
#include <memory>
#include <vector>

#include "BacktestEngine.h"
#include "PerformanceAnalyzer.h"
#include "MarketTradeLoader.h"
#include "MarketBarAggregator.h"
#include "MarketEvent.h"


int main() {

    // ==================================================
    // 1. LOAD RAW MARKET DATA
    // ==================================================

    auto marketTrades =
        MarketTradeLoader::loadCSV(
            "../../project1-odte-market-engine/data/etf_trades_large.csv"
        );

    std::cout << "Loaded Market Trades: "
              << marketTrades.size()
              << '\n';


    // ==================================================
    // 2. VALIDATE TRADE ORDERING
    // ==================================================

    std::size_t outOfOrderCount = 0;

    for (std::size_t i = 1;
         i < marketTrades.size();
         ++i) {

        if (marketTrades[i].getTimestamp() <
            marketTrades[i - 1].getTimestamp()) {

            ++outOfOrderCount;
        }
    }

    std::cout << "Out-of-Order Trades: "
              << outOfOrderCount
              << '\n';


    // ==================================================
    // 3. AGGREGATE TRADES INTO 5-MINUTE BARS
    // ==================================================

    auto marketBars =
        MarketBarAggregator::aggregateFiveMinuteBars(
            marketTrades
        );

    std::cout << "Aggregated 5-Minute Bars: "
              << marketBars.size()
              << '\n';


    // ==================================================
    // 4. VALIDATE VOLUME CONSERVATION
    //
    // Total volume in the raw trades should equal total
    // volume in the aggregated bars.
    // ==================================================

    long long totalTradeVolume = 0;

    for (const auto& trade : marketTrades) {
        totalTradeVolume += trade.getSize();
    }


    long long totalBarVolume = 0;

    for (const auto& bar : marketBars) {
        totalBarVolume += bar.getVolume();
    }


    std::cout << "Total Trade Volume: "
              << totalTradeVolume
              << '\n';

    std::cout << "Total Bar Volume: "
              << totalBarVolume
              << '\n';


    // ==================================================
    // 5. DISPLAY FIRST AND LAST AGGREGATED BARS
    //
    // These provide a quick sanity check of the
    // aggregation pipeline.
    // ==================================================

    if (!marketBars.empty()) {

        const MarketBar& firstBar =
            marketBars.front();

        const MarketBar& lastBar =
            marketBars.back();


        std::cout << "\nFirst Aggregated Bar:\n";

        std::cout << "Timestamp: "
                  << firstBar.getTimestamp()
                  << '\n';

        std::cout << "Symbol: "
                  << firstBar.getSymbol()
                  << '\n';

        std::cout << "Open: "
                  << firstBar.getOpen()
                  << '\n';

        std::cout << "High: "
                  << firstBar.getHigh()
                  << '\n';

        std::cout << "Low: "
                  << firstBar.getLow()
                  << '\n';

        std::cout << "Close: "
                  << firstBar.getClose()
                  << '\n';

        std::cout << "Volume: "
                  << firstBar.getVolume()
                  << '\n';


        std::cout << "\nLast Aggregated Bar:\n";

        std::cout << "Timestamp: "
                  << lastBar.getTimestamp()
                  << '\n';

        std::cout << "Symbol: "
                  << lastBar.getSymbol()
                  << '\n';

        std::cout << "Open: "
                  << lastBar.getOpen()
                  << '\n';

        std::cout << "High: "
                  << lastBar.getHigh()
                  << '\n';

        std::cout << "Low: "
                  << lastBar.getLow()
                  << '\n';

        std::cout << "Close: "
                  << lastBar.getClose()
                  << '\n';

        std::cout << "Volume: "
                  << lastBar.getVolume()
                  << '\n';
    }


    // ==================================================
    // 6. CONVERT MARKET BARS INTO MARKET EVENTS
    // ==================================================

    std::vector<std::shared_ptr<MarketEvent>>
        marketEvents;

    marketEvents.reserve(
        marketBars.size()
    );


    for (const MarketBar& bar : marketBars) {

        marketEvents.push_back(
            std::make_shared<MarketEvent>(
                bar.getTimestamp(),
                bar.getSymbol(),
                bar.getOpen(),
                bar.getHigh(),
                bar.getLow(),
                bar.getClose(),
                bar.getVolume()
            )
        );
    }


    std::cout << "\nMarket Events: "
              << marketEvents.size()
              << '\n';


    // ==================================================
    // 7. RUN EVENT-DRIVEN BACKTEST
    // ==================================================

    BacktestEngine engine;


    for (const auto& marketEvent : marketEvents) {
        engine.processMarketEvent(
            marketEvent
        );
    }


    // Discard an unexecutable final pending order and
    // liquidate any position that remains open.

    engine.closeOpenPosition();


    // ==================================================
    // 8. CALCULATE PERFORMANCE
    // ==================================================

    const auto& equityCurve =
        engine.getEquityCurve();

    const auto& completedTrades =
        engine.getCompletedTrades();


    auto returns =
        PerformanceAnalyzer::calculateReturns(
            equityCurve
        );


    double averageReturn =
        PerformanceAnalyzer::calculateAverageReturn(
            returns
        );

    double volatility =
        PerformanceAnalyzer::calculateVolatility(
            returns
        );

    double sharpeRatio =
        PerformanceAnalyzer::calculateSharpeRatio(
            returns
        );

    double maxDrawdown =
        PerformanceAnalyzer::calculateMaxDrawdown(
            equityCurve
        );

    double totalReturn =
        PerformanceAnalyzer::calculateTotalReturn(
            equityCurve
        );

    double positivePeriodRate =
        PerformanceAnalyzer::calculateWinRate(
            returns
        );


    // ==================================================
    // 9. BUY-AND-HOLD BENCHMARK
    //
    // This is the SPY price return from the first
    // market-event close to the final market-event close.
    // ==================================================

    double benchmarkReturn = 0.0;

    if (!marketEvents.empty()) {

        double initialPrice =
            marketEvents.front()->getClose();

        double finalPrice =
            marketEvents.back()->getClose();

        benchmarkReturn =
            (finalPrice / initialPrice) - 1.0;
    }


    double excessReturn =
        totalReturn - benchmarkReturn;


    // ==================================================
    // 10. TRADE-LEVEL PERFORMANCE
    // ==================================================

    std::size_t tradeCount =
        PerformanceAnalyzer::calculateTradeCount(
            completedTrades
        );

    double tradeWinRate =
        PerformanceAnalyzer::calculateTradeWinRate(
            completedTrades
        );

    double averageTradePnL =
        PerformanceAnalyzer::calculateAverageTradePnL(
            completedTrades
        );

    double profitFactor =
        PerformanceAnalyzer::calculateProfitFactor(
            completedTrades
        );

    double totalNetPnL =
        PerformanceAnalyzer::calculateTotalNetPnL(
            completedTrades
        );

    double averageWinningTrade =
        PerformanceAnalyzer::calculateAverageWinningTrade(
            completedTrades
        );

    double averageLosingTrade =
        PerformanceAnalyzer::calculateAverageLosingTrade(
            completedTrades
        );


    // ==================================================
    // 11. DISPLAY PERFORMANCE SUMMARY
    // ==================================================

    std::cout << "\n========================================\n";
    std::cout << "BACKTEST PERFORMANCE SUMMARY\n";
    std::cout << "========================================\n";

    std::cout << "Average Return: "
              << averageReturn
              << '\n';

    std::cout << "Volatility: "
              << volatility
              << '\n';

    std::cout << "Sharpe Ratio: "
              << sharpeRatio
              << '\n';

    std::cout << "Maximum Drawdown: "
              << maxDrawdown
              << '\n';

    std::cout << "Total Return: "
              << totalReturn
              << '\n';

    std::cout << "Buy-and-Hold Return: "
              << benchmarkReturn
              << '\n';

    std::cout << "Excess Return vs Buy-and-Hold: "
              << excessReturn
              << '\n';

    std::cout << "Positive Period Rate: "
              << positivePeriodRate
              << '\n';


    std::cout << "\n========================================\n";
    std::cout << "TRADE PERFORMANCE SUMMARY\n";
    std::cout << "========================================\n";

    std::cout << "Trade Count: "
              << tradeCount
              << '\n';

    std::cout << "Trade Win Rate: "
              << tradeWinRate
              << '\n';

    std::cout << "Average Trade P&L: "
              << averageTradePnL
              << '\n';

    std::cout << "Profit Factor: "
              << profitFactor
              << '\n';

    std::cout << "Total Net P&L: "
              << totalNetPnL
              << '\n';

    std::cout << "Average Winning Trade: "
              << averageWinningTrade
              << '\n';

    std::cout << "Average Losing Trade: "
              << averageLosingTrade
              << '\n';


    // ==================================================
    // 12. DISPLAY COMPLETED TRADES
    // ==================================================

    std::cout << "\n========================================\n";
    std::cout << "COMPLETED TRADES\n";
    std::cout << "========================================\n";


    std::size_t tradeNumber = 1;

    for (const Trade& trade : completedTrades) {

        std::cout << "\nTrade "
                  << tradeNumber
                  << '\n';

        std::cout << "Symbol: "
                  << trade.getSymbol()
                  << '\n';

        std::cout << "Entry Time: "
                  << trade.getEntryTimestamp()
                  << '\n';

        std::cout << "Exit Time: "
                  << trade.getExitTimestamp()
                  << '\n';

        std::cout << "Entry Price: "
                  << trade.getEntryPrice()
                  << '\n';

        std::cout << "Exit Price: "
                  << trade.getExitPrice()
                  << '\n';

        std::cout << "Quantity: "
                  << trade.getQuantity()
                  << '\n';

        std::cout << "Gross P&L: "
                  << trade.getGrossPnL()
                  << '\n';

        std::cout << "Net P&L: "
                  << trade.getNetPnL()
                  << '\n';

        std::cout << "Trade Return: "
                  << trade.getReturn()
                  << '\n';

        ++tradeNumber;
    }


    return 0;
}