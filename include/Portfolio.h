#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <memory>
#include <string>
#include <vector>

#include "SignalEvent.h"
#include "OrderEvent.h"
#include "FillEvent.h"
#include "Trade.h"


class Portfolio {
private:
    double cash;
    int position;

    std::string entryTimestamp;
    double entryPrice;
    double entryCommission;

    std::vector<Trade> completedTrades;


public:
    explicit Portfolio(double initialCash = 100000.0);

    std::shared_ptr<OrderEvent> generateOrder(
        const std::shared_ptr<SignalEvent>& signalEvent
    );

    // Check whether the portfolio has enough cash
    // to execute a BUY order at the supplied price.
    bool canExecuteOrder(
        const std::shared_ptr<OrderEvent>& orderEvent,
        double executionPrice,
        double estimatedCommission = 1.0
    ) const;

    void updateFill(
        const std::shared_ptr<FillEvent>& fillEvent
    );

    double getCash() const;
    int getPosition() const;

    double getMarketValue(double marketPrice) const;
    double getTotalEquity(double marketPrice) const;

    const std::vector<Trade>& getCompletedTrades() const;
};

#endif