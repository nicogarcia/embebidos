#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_
#include "LiquidCrystal.h"

class UserInterface_ {
public:
    void printModeAndTime(int mode, unsigned long time);

    void printTime(unsigned long time);

    void printBright(int bright);

    void printNoSavedTimes();
    void printTimeLCD(unsigned long time);
    static LiquidCrystal lcd;

};
extern UserInterface_ UserInterface;

#endif /* USERINTERFACE_H_ */