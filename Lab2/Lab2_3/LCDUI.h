#ifndef LCDUI_H_
#define LCDUI_H_
#include "LiquidCrystal.h"
#include "StopwatchState.h"

class LCDUI {
public:
    LCDUI();
    void updateUI();

    static LiquidCrystal screen;
private:
    // Time structure and function
    typedef struct {
        long minutes;
        int seconds;
        int hundreths;
    } UI_Time;
    static UI_Time ms_to_time(unsigned long time_ms);

    static const int MODES_COUNT = 5;

    // Store mode indication phrase
    static const char* MODE_NAMES[MODES_COUNT];

    // Stopwatch latest printed state
    StopwatchState* latest_state;

    // Stopwatch latest printed time
    unsigned long latest_time;

    // LCD 16x2 display will be divided in two lines of 14x1
    // and a 2x2 square for key states
    static const int LINE_LENGHT = 14;

    // LCD Lines, define with space for null terminating char
    static char first_line[LINE_LENGHT + 1];
    static char second_line[LINE_LENGHT + 1];

    // LCD Key states variables
    enum KeyStatePosition {
        UI_KEY_POSITION_UP = 0,
        UI_KEY_POSITION_SELECT = 1,
        UI_KEY_POSITION_DOWN = 2,
        UI_KEY_POSITION_LONG = 3
    };
    char key_state[4];
    /*
    // Key states
    enum UIKeys {
        UIKEY_SELECT = 0,
        UIKEY_DOWN = 1,
        UIKEY_UP = 2
    };
    int key_states[3];
    bool
    */
    void printFirstLine();
    void printSecondLine();
    void printKeyState();
    void LCDPrint();
    void SerialPrint();

    void buildFirstLine();

    void (*second_line_builders[MODES_COUNT])();
    static void MP_buildSecondLine();
    static void MCA_buildSecondLine();
    static void MCD_buildSecondLine();
    static void MVT_buildSecondLine();
    static void MAD_buildSecondLine();

    // Utilities
    static void my_strcpy(const char* source, char* destiny);
    static void clear_line(char* source);
    static void printTime(UI_Time time, char* destiny, int position);
    //
    // TODO: Special characters

};
extern LCDUI lcd_ui;
#endif /* LCDUI_H_ */