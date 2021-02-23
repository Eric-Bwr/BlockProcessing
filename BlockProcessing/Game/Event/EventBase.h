#pragma once

struct EventBase {
public:
    virtual void processEvent() = 0;
    int id = 0;
};