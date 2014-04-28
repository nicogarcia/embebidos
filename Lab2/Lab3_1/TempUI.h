#ifndef UI_H_
#define UI_H_
#include "LCDUI.h"

class UI_ : public LCDUI {
public:
    // Printing methods
    void printMessage() { };
    void printKeyState() { };

    // UI Building Functions
    void buildFirstLine();
    void buildSecondLine();
};
extern UI_ ui;

#endif /* UI_H_ */