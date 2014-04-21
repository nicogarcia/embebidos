#include "Arduino.h"
#include "UserInterface.h"
#include "SoftwareSerial.h"
#include "Stopwatch.h"
#include "LiquidCrystal.h"

UserInterface_ UserInterface;

// initialize the library with the numbers of the interface pins
LiquidCrystal UserInterface_::lcd(8, 9, 4, 5, 6, 7);

const char* MODE_NAMES[] = { "MP  MOD", "MCA MOD", "MCD MOD", "MAD MOD", "MVT MOD"};

int last_minutes = -1, last_seconds = -1, last_hundreths = -1;
int last_mode = -1;

int minutes = 0, seconds = 0, hundreths = 0;
void ms_to_time(unsigned long time_ms) {
    hundreths = (time_ms / 10) % 100;
    seconds = (time_ms / 1000) % 60;
    minutes = time_ms / 60000;
}

void UserInterface_::printModeAndTime(int mode, unsigned long time) {
    // Fill min sec cent with time
    ms_to_time(time);

    // Don't print again
    if(Stopwatch.getCurrentState() == Stopwatch.last_state)
        return;
    // FIXME
    // Update initial Stopwatch state
    if(Stopwatch.last_state == NULL)
        Stopwatch.last_state = &Stopwatch.MP;

    lcd.setCursor(0, 0);
    lcd.print(MODE_NAMES[mode]);
    lcd.setCursor(8, 0);
    lcd.print(":");
    printTimeLCD(time);

    Serial.print(MODE_NAMES[mode]);
    Serial.print(": ");
    printTime(time);
}

void UserInterface_::printTimeLCD(unsigned long time) {
    ms_to_time(time);

    // Print minutes with padding
    lcd.setCursor(0, 1);
    if(minutes < 10)
        lcd.print("0");
    lcd.setCursor(1, 1);
    lcd.print(minutes);

    // Print minute-second separator
    lcd.setCursor(2, 1);
    lcd.print(":");

    // Print seconds with padding
    lcd.setCursor(3, 1);
    if(seconds < 10)
        lcd.print("0");
    lcd.print(seconds);

    // Print seconds-hundreths separator
    lcd.setCursor(5, 1);
    lcd.print(".");

    // Print hundreths with padding
    lcd.setCursor(6, 1);
    if(hundreths < 10)
        lcd.print("0");
    lcd.print(hundreths);
}

void UserInterface_::printTime(unsigned long time) {
    ms_to_time(time);

    // Print minutes with padding
    if(minutes < 10)
        Serial.print("0");
    Serial.print(minutes);

    // Print minute-second separator
    Serial.print(":");

    // Print seconds with padding
    if(seconds < 10)
        Serial.print("0");
    Serial.print(seconds);

    // Print seconds-hundreths separator
    Serial.print(".");

    // Print hundreths with padding
    if(hundreths < 10)
        Serial.print("0");
    Serial.println(hundreths);
}

int last_bright = -1;
void UserInterface_::printBright( int bright ) {
    //if((Stopwatch.getCurrentState() != Stopwatch.last_state) && (last_bright != bright)) {
    Serial.print("MAD MODE: Bright: ");
    Serial.print(bright);
    Serial.println("%");

    lcd.setCursor(0, 0);
    lcd.print(MODE_NAMES[Stopwatch_::MODE_MAD]);
    lcd.setCursor(8, 0);
    lcd.print(":");
    lcd.setCursor(0, 1);
    lcd.print(bright);
    lcd.setCursor(3, 1);
    // TODO: FIX THIS
    lcd.print("%    ");
    //}
    last_bright = bright;
}

void UserInterface_::printNoSavedTimes() {
    if(Stopwatch.getCurrentState() != Stopwatch.last_state)
        Serial.println("MVT MODE: No saved times.");
}

