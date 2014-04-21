#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_

class UserInterface_ {
public:
    void printModeAndTime(int mode, unsigned long time);

    void printTime(unsigned long time);

    void printBright(int bright);

    void printNoSavedTimes();
private:

};
extern UserInterface_ UserInterface;

#endif /* USERINTERFACE_H_ */