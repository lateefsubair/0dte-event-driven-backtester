#ifndef BACKTEST_ENGINE_H
#define BACKTEST_ENGINE_H

#include <memory>
#include <string>
#include <vector>

#include "EventQueue.h"
#include "Strategy.h"
#include "Portfolio.h"
#include "ExecutionHandler.h"
#include "MarketEvent.h"
#include "OrderEvent.h"
#include "Trade.h"

class BacktestEngine {
private:
    EventQueue eventQueue;
    Strategy strategy;
    Portfolio portfolio;
    ExecutionHandler executionHandler;


    double latestMarketPrice;
    // Information from the most recently processed
    // market event.
    std::string latestMarketTimestamp;
    std::string latestSymbol;

    // Order generated from the previous completed bar.
    // It waits until the NEXT market bar arrives,
    // then executes at that bar's OPEN price.
    std::shared_ptr<OrderEvent> pendingOrder;

    std::vector<double> equityCurve;

public:
    BacktestEngine();

    void processMarketEvent(
        const std::shared_ptr<MarketEvent>& marketEvent
    );

    const std::vector<double>& getEquityCurve() const;
    const std::vector<Trade>& getCompletedTrades() const;

    int getPosition() const;

    void closeOpenPosition();

private:
    void processEventQueue();
};

#endif