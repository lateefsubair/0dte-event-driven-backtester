#include "SignalEvent.h"

SignalEvent::SignalEvent(
    const std::string& timestamp,
    const std::string& symbol,
    SignalType signalType,
    double strength
)
    : timestamp(timestamp),
      symbol(symbol),
      signalType(signalType),
      strength(strength) {
}

EventType SignalEvent::getType() const {
    return EventType::SIGNAL;
}

const std::string& SignalEvent::getTimestamp() const {
    return timestamp;
}

const std::string& SignalEvent::getSymbol() const {
    return symbol;
}

SignalType SignalEvent::getSignalType() const {
    return signalType;
}

double SignalEvent::getStrength() const {
    return strength;
}