#pragma once

struct ChatComponent {
    ChatComponent() = default;
    virtual void init(){}
    virtual void hover(){}
    virtual void click(){}

};