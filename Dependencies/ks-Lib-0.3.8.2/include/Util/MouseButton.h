// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once

//Namespace InputBuffer
namespace ib {
    class MouseButton {
    public:
        MouseButton() {}
        inline void setButtonState(bool state) { ButtonPressed = state; }
        inline bool getButtonState() { return ButtonPressed; }
    private:
        bool ButtonPressed = false;
    };
} //Namespace InputBuffer