#include "OrderEvent.h"

OrderEvent::OrderEvent(
    const std::string& timestamp,
    const std::string& symbol,
    OrderType orderType,
    OrderSide side,
    int quantity,
    double limitPrice
)
    : timestamp(timestamp),
      symbol(symbol),
      orderType(orderType),
      side(side),
      quantity(quantity),
      limitPrice(limitPrice) {
}

EventType OrderEvent::getType() const {
    return EventType::ORDER;
}

const std::string& OrderEvent::getTimestamp() const {
    return timestamp;
}

const std::string& OrderEvent::getSymbol() const {
    return symbol;
}

OrderType OrderEvent::getOrderType() const {
    return orderType;
}

OrderSide OrderEvent::getSide() const {
    return side;
}

int OrderEvent::getQuantity() const {
    return quantity;
}

double OrderEvent::getLimitPrice() const {
    return limitPrice;
}