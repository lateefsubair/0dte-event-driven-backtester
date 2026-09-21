#ifndef EXECUTION_HANDLER_H
#define EXECUTION_HANDLER_H

#include <memory>
#include <string>

#include "OrderEvent.h"
#include "FillEvent.h"


class ExecutionHandler {
public:
    std::shared_ptr<FillEvent> executeOrder(
        const std::shared_ptr<OrderEvent>& orderEvent,
        double marketPrice,
        const std::string& executionTimestamp
    );
};

#endif