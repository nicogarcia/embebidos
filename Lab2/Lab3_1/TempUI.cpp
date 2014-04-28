#include "TempUI.h"
#include "TempMonitor.h"

UI_ ui;

void UI_::buildFirstLine() {
    my_strcpy(TempMonitor.state_names[TempMonitor.current_state],
              first_line);
}
void UI_::buildSecondLine() {
    const int padding = 7;
    double_to_str(TempMonitor.data[TempMonitor.current_state], second_line + padding);
}