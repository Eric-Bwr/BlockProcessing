// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once

//Namespace InputBuffer
namespace ib {
    class Key {
    public:
        Key() = default;
        Key(int key) : keyBind(key) {}
        inline bool checkKey(int key) { if(key == keyBind) {keyPressed = !keyPressed; return true; } return false; }
        inline bool getKeyState() { return keyPressed; }
    private:
        int keyBind;
        bool keyPressed = false;
    };
} //Namespace InputBuffer
