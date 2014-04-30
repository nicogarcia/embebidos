#include "TempUI.h"
#include "TempMonitor.h"

TempUI_ ui;

void TempUI_::buildFirstLine() {
    my_strcpy(TempMonitor.state_names[TempMonitor.current_state],
              first_line);
}

void TempUI_::buildSecondLine() {
    const int padding = 3;
    double_to_str(TempMonitor.data[TempMonitor.current_state], second_line + padding);
    *(second_line + padding + 5) = '°';
    *(second_line + padding + 6) = 'C';
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
    if(TempMonitor.data[TempMonitor.STATE_CURRENT_TEMP] > HIGH_TEMP_THRES) {
        //clear_line(first_line);
        my_strcpy(" TEMP ELEVADA! ", first_line);
    }
}