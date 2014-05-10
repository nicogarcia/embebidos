#ifndef UI_H_
#define UI_H_
#include "LCDUI.h"

class TempUI_ : public LCDUI {
public:

    enum i2c_modes {
        NONE = 0,
        MASTER = 1,
        SLAVE = 2
    };

    static int i2c_mode;

    // High temperature threshold
    static const double HIGH_TEMP_THRES = 60;

    // Printing methods
    void printMessage();
    void printKeyState();

    // UI Building Functions
    void buildFirstLine();
    void buildSecondLine();
    void printTemperature(int state);
};
extern TempUI_ ui;

#endif /* UI_H_ */