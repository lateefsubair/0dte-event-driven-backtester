#include "EventQueue.h"

void EventQueue::push(const std::shared_ptr<Event>& event) {
    events.push(event);
}

std::shared_ptr<Event> EventQueue::pop() {
    if (events.empty()) {
        return nullptr;
    }

    auto event = events.front();
    events.pop();

    return event;
}

bool EventQueue::empty() const {
    return events.empty();
}

std::size_t EventQueue::size() const {
    return events.size();
}