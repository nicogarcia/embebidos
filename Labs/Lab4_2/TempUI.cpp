#include "TempUI.h"
#include "TempMonitor.h"

TempUI_ ui;
int TempUI_::i2c_mode = NONE;

void TempUI_::buildFirstLine() {
    my_strcpy(TempMonitor.state_names[TempMonitor.current_state],
              first_line);
}

void TempUI_::printTemperature(int state) {
    const int padding = 3;
    double_to_str(TempMonitor.data[state], second_line + padding);
    *(second_line + padding + 5) = DEGREES_CHAR;
    *(second_line + padding + 6) = 'C';
}

void TempUI_::buildSecondLine() {
    printTemperature(TempMonitor.current_state);
}

void TempUI_::printKeyState() {
    if(TempMonitor.up_key)
        first_line[UI_KEY_POSITION_SELECT] = UP_ARROW_FILLED_CHAR;
    else
        first_line[UI_KEY_POSITION_SELECT] = UP_ARROW_BORDERS_CHAR;

    if(TempMonitor.down_key)
        second_line[UI_KEY_POSITION_LONG] = DOWN_ARROW_FILLED_CHAR;
    else
        second_line[UI_KEY_POSITION_LONG] = DOWN_ARROW_BORDERS_CHAR;
}

void TempUI_::printMessage() {
    if(!messages_disabled) {
        clear_line(first_line);
        clear_line(second_line);
        my_strcpy("EMBEBIDOS  LAB 4", first_line);
        my_strcpy(" 1er Cuat COM10 ", second_line);
    }

    if(TempMonitor.data[TempMonitor.STATE_CURRENT_TEMP] > HIGH_TEMP_THRES) {
        my_strcpy(" TEMP ELEVADA! ", first_line);
        printTemperature(TempMonitor.STATE_CURRENT_TEMP);
    }
}