#include "Strategy.h"

std::shared_ptr<SignalEvent> Strategy::generateSignal(
    const std::shared_ptr<MarketEvent>& marketEvent
) {
    // --------------------------------------------------
    // MOMENTUM STRATEGY
    // --------------------------------------------------
    //
    // Measure the return of the current 5-minute bar:
    //
    //     barReturn = (Close - Open) / Open
    //
    // We only enter a LONG position when the bar has
    // sufficiently strong positive momentum.
    //
    // A small threshold prevents tiny price movements
    // from automatically generating LONG signals.
    // --------------------------------------------------

    double openPrice = marketEvent->getOpen();
    double closePrice = marketEvent->getClose();

    // Protect against invalid data / division by zero.
    if (openPrice <= 0.0) {
        return nullptr;
    }

    double barReturn =
        (closePrice - openPrice) / openPrice;

    // Require at least a 0.10% positive move
    // during the 5-minute bar.
    const double momentumThreshold = 0.001;

    if (barReturn > momentumThreshold) {

        return std::make_shared<SignalEvent>(
            marketEvent->getTimestamp(),
            marketEvent->getSymbol(),
            SignalType::LONG,
            barReturn
        );
    }

    // If momentum is not sufficiently positive,
    // exit any existing long position.
    return std::make_shared<SignalEvent>(
        marketEvent->getTimestamp(),
        marketEvent->getSymbol(),
        SignalType::EXIT,
        1.0
    );
}