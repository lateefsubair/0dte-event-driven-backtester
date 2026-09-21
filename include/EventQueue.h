#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <queue>
#include <memory>

#include "Event.h"

class EventQueue {
private:
    std::queue<std::shared_ptr<Event>> events;

public:
    void push(const std::shared_ptr<Event>& event);

    std::shared_ptr<Event> pop();

    bool empty() const;

    std::size_t size() const;
};

#endif