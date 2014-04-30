#ifndef UI_H_
#define UI_H_
#include "LCDUI.h"

class TempUI_ : public LCDUI {
public:
    // High temperature threshold
    static const double HIGH_TEMP_THRES = 32;

    // Printing methods
    void printMessage();
    void printKeyState();

    // UI Building Functions
    void buildFirstLine();
    void buildSecondLine();
};
extern TempUI_ ui;

#endif /* UI_H_ */