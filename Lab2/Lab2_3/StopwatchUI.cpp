#include "StopwatchUI.h"
#include "KeyManagement.h"
#include "Stopwatch.h"

UI_ ui;

const char* UI_::MODE_NAMES[MODES_COUNT] = {
    "   EN PAUSA   ",
    "  CRONOMETRO  ",
    " TEMPORIZADOR ",
    "   REGISTRO   ",
    "    BRILLO    ",
    "EMBEBIDOS  LAB 2"
};

UI_::UI_() {
    second_line_builders[Stopwatch.STATE_MP] = &UI_::MP_buildSecondLine;
    second_line_builders[Stopwatch.STATE_MCA] = &UI_::MCA_buildSecondLine;
    second_line_builders[Stopwatch.STATE_MCD] = &UI_::MCD_buildSecondLine;
    second_line_builders[Stopwatch.STATE_MVT] = &UI_::MVT_buildSecondLine;
    second_line_builders[Stopwatch.STATE_MAD] = &UI_::MAD_buildSecondLine;
    second_line_builders[Stopwatch.STATE_INIT] = &UI_::INIT_buildSecondLine;

    print_message = &UI_::empty_function_2;
}


void UI_::buildFirstLine() {
    my_strcpy(MODE_NAMES[Stopwatch.getCurrentStatePointer()], first_line);
}

void UI_::buildSecondLine() {
    (this->*second_line_builders[Stopwatch.getCurrentStatePointer()])();
}

/*************************************************/
//	Second line bui_lders
/*************************************************/

void UI_::INIT_buildSecondLine() {
    my_strcpy(" 1er Cuat COM10 ", second_line);
}

void UI_::MP_buildSecondLine() {
    printTime(ms_to_time(Stopwatch.current_time), second_line, 3);
}

void UI_::MCA_buildSecondLine() {
    printTime(ms_to_time(Stopwatch.current_time), second_line, 3);
}

void UI_::MCD_buildSecondLine() {
    printTime(ms_to_time(Stopwatch.current_time), second_line, 3);
}

void UI_::MVT_buildSecondLine() {
    unsigned long int time = Stopwatch.times[Stopwatch.viewing_index];
    if(Stopwatch.times_count == 0)
        my_strcpy("    Vacio      ", second_line);
    else
        printTime(ms_to_time(time), second_line, 3);
}

void UI_::MAD_buildSecondLine() {
    int pos = 5;
    bool is_100 = Stopwatch.current_bright / 100;
    bool is_0 = Stopwatch.current_bright == 0;
    second_line[pos++] = is_100 ? '1' : ' ';
    second_line[pos++] = is_0 ? ' ' : int_to_char_num((Stopwatch.current_bright % 100) / 10);
    second_line[pos++] = int_to_char_num(Stopwatch.current_bright % 10);
    second_line[pos++] = '%';
}
void UI_::print_no_save() {
    my_strcpy(" NO HAY TIEMPOS ", first_line);
    my_strcpy("   GUARDADOS    ", second_line);
}

void UI_::print_saved_time() {
    clear_line(second_line);
    printTime(ms_to_time(time_saved), second_line, 4);
    my_strcpy("TIEMPO GUARDADO ", first_line);
}

void no_print() {
    ui.print_message = &UI_::empty_function_2;
}

void UI_::show_no_saved_message() {
    print_message = &UI_::print_no_save;
    SystemClock.attach(Task(1500, no_print));
}

void UI_::show_saved_time_message(unsigned long time) {
    print_message = &UI_::print_saved_time;
    time_saved = time;
    SystemClock.attach(Task(1500, no_print));
}

void UI_::printKeyState() {
    // Don't print keys in the welcome screen
    if(Stopwatch.getCurrentStatePointer() == Stopwatch.STATE_INIT)
        return;

    if(KeyManagement.key_states[KeyManagement.KEY_STATE_UP])
        first_line[UI_KEY_POSITION_UP] = UP_ARROW_FILLED_CHAR;
    else
        first_line[UI_KEY_POSITION_UP] = UP_ARROW_BORDERS_CHAR;

    if(KeyManagement.key_states[KeyManagement.KEY_STATE_SELECT])
        first_line[UI_KEY_POSITION_SELECT] = 'S';
    else
        first_line[UI_KEY_POSITION_SELECT] = ' ';

    if(KeyManagement.key_states[KeyManagement.KEY_STATE_DOWN])
        second_line[UI_KEY_POSITION_DOWN] = DOWN_ARROW_FILLED_CHAR;
    else
        second_line[UI_KEY_POSITION_DOWN] = DOWN_ARROW_BORDERS_CHAR;

    if(KeyManagement.key_states[KeyManagement.KEY_STATE_LONG])
        second_line[UI_KEY_POSITION_LONG] = 'L';
    else
        second_line[UI_KEY_POSITION_LONG] = ' ';
}

void UI_::printMessage() {
    (this->*print_message)();
}
