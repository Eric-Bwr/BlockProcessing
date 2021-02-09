// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once
#include "Event.h"

//Namespace EventSystem
namespace es {
    class EventDispatcher{
    public:
        EventDispatcher(Event& event) : event(event) {}

        template<typename T>
        bool dispatch(bool (*function)(T&)) {
            if(event.getEventID() == T::GetEventID()) {
                event.handled = function(static_cast<T&>(event));
                return true;
            }
            return false;
        }

        template<typename T, typename F>
        bool dispatchMFunc(const F &func) {
            if(event.getEventID() == T::GetEventID()) {
                event.handled = func(static_cast<T&>(event));
                return true;
            }
            return false;
        }
    private:
        Event& event;
    };
} //Namespace EventSystem
