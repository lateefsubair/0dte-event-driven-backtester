#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <queue>
#include <memory>

#include "MarketEvent.h"

class EventQueue {
private:
    std::queue<std::shared_ptr<MarketEvent>> events;

public:
    void push(const std::shared_ptr<MarketEvent>& event);

    std::shared_ptr<MarketEvent> pop();

    bool empty() const;

    std::size_t size() const;
};

#endif