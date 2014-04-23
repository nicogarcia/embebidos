#include "Arduino.h"
#include "SoftwareSerial.h"
#include "Stopwatch.h"
#include "LiquidCrystal.h"
#include "LCDUI.h"

// LCDUI instance
LCDUI lcd_ui;

char LCDUI::first_line[LINE_LENGHT + 1];
char LCDUI::second_line[LINE_LENGHT + 1];

// initialize the library with the numbers of the interface pins
LiquidCrystal LCDUI::screen = LiquidCrystal(8, 9, 4, 5, 6, 7);

const char* LCDUI::MODE_NAMES[MODES_COUNT] = {
    "   PAUSADO",
    "  CRONOMETRO",
    " TEMPORIZADOR",
    "   REGISTRO",
    "    BRILLO"
};

LCDUI::LCDUI() {
    first_line[LINE_LENGHT] = 0;
    second_line[LINE_LENGHT] = 0;

    for(int i = 0; i < 4; i++)
        key_state[i] = ' ';

    latest_time = 0;

    second_line_builders[Stopwatch.STATE_MP] = MP_buildSecondLine;
    second_line_builders[Stopwatch.STATE_MCA] = MCA_buildSecondLine;
    second_line_builders[Stopwatch.STATE_MCD] = MCD_buildSecondLine;
    second_line_builders[Stopwatch.STATE_MVT] = MVT_buildSecondLine;
    second_line_builders[Stopwatch.STATE_MAD] = MAD_buildSecondLine;
}

void LCDUI::updateUI() {
    // TODO: Update if necessary

    printFirstLine();
    printSecondLine();
    //printKeyState();

    // LCDPrint();
    SerialPrint();
}

void LCDUI::LCDPrint() {
    // Actually print first line
    screen.setCursor(0, 0);
    screen.print(first_line);

    // Actually print second line
    screen.setCursor(0, 1);
    screen.print(second_line);

    // Actually print key states
    // Print up and select key states
    screen.setCursor(LINE_LENGHT, 0);
    screen.print(key_state[UI_KEY_POSITION_UP]);
    screen.setCursor(LINE_LENGHT + 1, 0);
    screen.print(key_state[UI_KEY_POSITION_SELECT]);

    // Print down and long key states
    screen.setCursor(LINE_LENGHT, 1);
    screen.print(key_state[UI_KEY_POSITION_DOWN]);
    screen.setCursor(LINE_LENGHT + 1, 1);
    screen.print(key_state[UI_KEY_POSITION_LONG]);
}

void LCDUI::SerialPrint() {
    // Print first line through Serial
    Serial.print(first_line);
    Serial.print(key_state[UI_KEY_POSITION_UP]);
    Serial.println(key_state[UI_KEY_POSITION_SELECT]);

    // Print second line through Serial
    Serial.print(second_line);
    Serial.print(key_state[UI_KEY_POSITION_DOWN]);
    Serial.println(key_state[UI_KEY_POSITION_LONG]);

}

LCDUI::UI_Time LCDUI::ms_to_time(unsigned long time_ms) {
    UI_Time time;
    time.hundreths = (time_ms / 10) % 100;
    time.seconds = (time_ms / 1000) % 60;
    time.minutes = (time_ms / 60000 % 100); // Modulo 100 to avoid overflow
    return time;
}

void LCDUI::my_strcpy(const char* source, char* destiny) {
    while((*destiny++ = *source++));
}
void LCDUI::clear_line(char* source) {
    for(int i = 0; i < LINE_LENGHT; i++)
        *(source + i) = ' ';
}

void LCDUI::printFirstLine() {
    clear_line(first_line);
    buildFirstLine();
}

void LCDUI::printSecondLine() {
    clear_line(second_line);
    second_line_builders[Stopwatch.getCurrentStatePointer()]();
}

// First line builder, common for all states
void LCDUI::buildFirstLine() {
    my_strcpy(MODE_NAMES[Stopwatch.getCurrentStatePointer()], first_line);
}

inline char int_to_char_num(int num) {
    return num + '0';
}

// Prints time in a string with the format MM:SS.CC in the specified position
void LCDUI::printTime(UI_Time time, char* destiny, int position) {
    int pos = position;

    // Write minutes
    destiny[pos++] = int_to_char_num(time.minutes / 10);
    destiny[pos++] = int_to_char_num(time.minutes % 10);

    // Write min sec separator
    destiny[pos++] = ':';

    // Write seconds
    destiny[pos++] = int_to_char_num(time.seconds / 10);
    destiny[pos++] = int_to_char_num(time.seconds % 10);

    // Write sec hund separator
    destiny[pos++] = '.';

    // Write hundreths
    destiny[pos++] = int_to_char_num(time.hundreths / 10);
    destiny[pos++] = int_to_char_num(time.hundreths % 10);
}

/*************************************************/
//	Second line builders
/*************************************************/

void LCDUI::MP_buildSecondLine() {
    printTime(ms_to_time(Stopwatch.current_time), second_line, 3);
}

void LCDUI::MCA_buildSecondLine() {
    printTime(ms_to_time(Stopwatch.current_time), second_line, 3);
}

void LCDUI::MCD_buildSecondLine() {
    printTime(ms_to_time(Stopwatch.current_time), second_line, 3);
}

void LCDUI::MVT_buildSecondLine() {

}

void LCDUI::MAD_buildSecondLine() {
    int pos = 5;
    bool is_100 = Stopwatch.current_bright / 100;
    second_line[pos++] = is_100 ? '1' : ' ';
    second_line[pos++] = int_to_char_num((Stopwatch.current_bright % 100) / 10);
    second_line[pos++] = int_to_char_num(Stopwatch.current_bright % 10);
    second_line[pos++] = '%';
}

