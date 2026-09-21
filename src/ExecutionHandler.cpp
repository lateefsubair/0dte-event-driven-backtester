#include "ExecutionHandler.h"


std::shared_ptr<FillEvent> ExecutionHandler::executeOrder(
    const std::shared_ptr<OrderEvent>& orderEvent,
    double marketPrice,
    const std::string& executionTimestamp
) {
    // --------------------------------------------------
    // SLIPPAGE MODEL
    //
    // Apply 1 basis point of adverse slippage.
    //
    // BUY:
    //     pay slightly MORE than the market price.
    //
    // SELL:
    //     receive slightly LESS than the market price.
    // --------------------------------------------------

    const double slippageBps = 1.0;

    const double slippageRate =
        slippageBps / 10000.0;

    double fillPrice =
        marketPrice;


    if (orderEvent->getSide() == OrderSide::BUY) {

        fillPrice =
            marketPrice *
            (1.0 + slippageRate);
    }


    if (orderEvent->getSide() == OrderSide::SELL) {

        fillPrice =
            marketPrice *
            (1.0 - slippageRate);
    }


    // --------------------------------------------------
    // Simple fixed commission model.
    // --------------------------------------------------

    const double commission =
        1.00;


    // --------------------------------------------------
    // IMPORTANT:
    //
    // A FillEvent represents when execution actually
    // occurred.
    //
    // Therefore we use executionTimestamp rather than
    // orderEvent->getTimestamp().
    // --------------------------------------------------

    return std::make_shared<FillEvent>(
        executionTimestamp,
        orderEvent->getSymbol(),
        orderEvent->getSide(),
        orderEvent->getQuantity(),
        fillPrice,
        commission
    );
}