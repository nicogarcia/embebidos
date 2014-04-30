#ifndef UI_H_
#define UI_H_
#include "LCDUI.h"

class UI_ : public LCDUI {
public:
    UI_();

    void show_no_saved_message();

    void show_saved_time_message(unsigned long save);

    // Message printing function
    void (UI_::*print_message)();

    void empty_function_2() {}
private:

    static const int MODES_COUNT = 6;
    // Store mode indication phrase
    static const char* MODE_NAMES[MODES_COUNT];
    unsigned long time_saved;

    void (UI_::*second_line_builders[MODES_COUNT])();

    // Second line builders, one for each state
    void MCA_buildSecondLine();
    void MCD_buildSecondLine();
    void MVT_buildSecondLine();
    void MAD_buildSecondLine();
    void MP_buildSecondLine();
    void INIT_buildSecondLine();

    // Messages
    void printMessage();
    void print_no_save();
    void print_saved_time();


    // LCDUI Virtual Functions
    void buildFirstLine();
    void buildSecondLine();
    void printKeyState();
};
extern UI_ ui;

#endif /* UI_H_ */