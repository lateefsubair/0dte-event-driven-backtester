#ifndef SIGNAL_EVENT_H
#define SIGNAL_EVENT_H

#include <string>
#include "Event.h"

enum class SignalType {
    LONG,
    SHORT,
    EXIT
};

class SignalEvent : public Event {
private:
    std::string timestamp;
    std::string symbol;
    SignalType signalType;
    double strength;

public:
    SignalEvent(
        const std::string& timestamp,
        const std::string& symbol,
        SignalType signalType,
        double strength
    );

    EventType getType() const override;
    const std::string& getTimestamp() const;
    const std::string& getSymbol() const;
    SignalType getSignalType() const;
    double getStrength() const;
};

#endif