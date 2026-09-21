#ifndef FILL_EVENT_H
#define FILL_EVENT_H

#include <string>
#include "Event.h"
#include "OrderEvent.h"

class FillEvent : public Event {
private:
    std::string timestamp;
    std::string symbol;
    OrderSide side;
    int quantity;
    double fillPrice;
    double commission;

public:
    FillEvent(
        const std::string& timestamp,
        const std::string& symbol,
        OrderSide side,
        int quantity,
        double fillPrice,
        double commission
    );

    EventType getType() const override;

    const std::string& getTimestamp() const;
    const std::string& getSymbol() const;
    OrderSide getSide() const;
    int getQuantity() const;
    double getFillPrice() const;
    double getCommission() const;
};

#endif