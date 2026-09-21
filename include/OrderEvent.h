#ifndef ORDER_EVENT_H
#define ORDER_EVENT_H

#include <string>
#include "Event.h"

enum class OrderType {
    MARKET,
    LIMIT
};

enum class OrderSide {
    BUY,
    SELL
};

class OrderEvent : public Event {
private:
    std::string timestamp;
    std::string symbol;
    OrderType orderType;
    OrderSide side;
    int quantity;
    double limitPrice;

public:
    OrderEvent(
        const std::string& timestamp,
        const std::string& symbol,
        OrderType orderType,
        OrderSide side,
        int quantity,
        double limitPrice = 0.0
    );

    EventType getType() const override;

    const std::string& getTimestamp() const;
    const std::string& getSymbol() const;
    OrderType getOrderType() const;
    OrderSide getSide() const;
    int getQuantity() const;
    double getLimitPrice() const;
};

#endif