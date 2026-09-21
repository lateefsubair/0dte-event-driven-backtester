#include "FillEvent.h"

FillEvent::FillEvent(
    const std::string& timestamp,
    const std::string& symbol,
    OrderSide side,
    int quantity,
    double fillPrice,
    double commission
)
    : timestamp(timestamp),
      symbol(symbol),
      side(side),
      quantity(quantity),
      fillPrice(fillPrice),
      commission(commission) {
}

EventType FillEvent::getType() const {
    return EventType::FILL;
}

const std::string& FillEvent::getTimestamp() const {
    return timestamp;
}

const std::string& FillEvent::getSymbol() const {
    return symbol;
}

OrderSide FillEvent::getSide() const {
    return side;
}

int FillEvent::getQuantity() const {
    return quantity;
}

double FillEvent::getFillPrice() const {
    return fillPrice;
}

double FillEvent::getCommission() const {
    return commission;
}