#ifndef EVENT_H
#define EVENT_H

enum class EventType {
    MARKET,
    SIGNAL,
    ORDER,
    FILL
};

class Event {
public:
    virtual ~Event() = default;

    virtual EventType getType() const = 0;
};

#endif