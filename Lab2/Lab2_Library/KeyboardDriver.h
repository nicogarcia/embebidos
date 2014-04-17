#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H

// Interface to be implemented by keyboard drivers
class KeyboardDriver {
public:
    // Unimplemented virtual methods, java equivalent of "interface"
    virtual void registerOnKeyDownCallback(void (*handler)(), int key) = 0;
    virtual void registerOnKeyUpCallback(void (*handler)(), int key) = 0;
};

#endif
