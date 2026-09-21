#ifndef STRATEGY_H
#define STRATEGY_H

#include <memory>

#include "MarketEvent.h"
#include "SignalEvent.h"

class Strategy {
public:
    std::shared_ptr<SignalEvent> generateSignal(
        const std::shared_ptr<MarketEvent>& marketEvent
    );
};

#endif