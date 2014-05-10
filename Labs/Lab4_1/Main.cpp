#include "Arduino.h"
#include "SoftwareSerial.h"
#include "SystemClock.h"
#include "LCDKeypadKeys.h"
#include "LCDKeypadDriver.h"
#include "TempUI.h"
#include "TempSensorDriver.h"
#include "TempMonitor.h"
#include "ADCManager.h"
#include "PriorityQueue.h"
#include "CommProtocol.h"

void setup() {
    ui.serial_enabled = false;
    // Initialize lcd screen
    ui.initScreen();

    lm35.initialize();
    KeypadDriver.initialize();

    ADCManager.insertDriver(&lm35, 1);
    ADCManager.insertDriver(&KeypadDriver, 0);

    //Inicializar el pin del led como salida
    DDRB |= (1 << DDB5);

    Serial.begin(115200);

    ui.updateUI();

    SystemClock.attach(Task(2000, ui.disable_message_print));
}

void loop() {
    // Check and execute if there were events triggered
    SystemClock.checkEvents();
}

void serialEvent() {
    // Read trash until START_TOKEN read
    while(true) {
        if(Serial.peek() == -1)
            return;
        if(Serial.peek() == CommProtocol.START_TOKEN)
            break;
        Serial.read();
    }

    TempMessage tm = CommProtocol.ReadMessage();

    int last_state = TempMonitor.current_state;

    TempMonitor.current_state = tm.mode - 10;

    // Update UI to see the new state, if so
    if(last_state != TempMonitor.current_state)
        ui.updateUI();
}
