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
#include "Wire.h"
#include "I2CComunication.h"

TempMessage msg;

void comm_send_msg() {
    CommProtocol.SendMessage(msg);
}

void communication(int size) {
    //Slave arduino
    msg = I2CComunication.I2CReadMessage();

    msg.mode = TempMonitor.current_state + 10;
    TempMonitor.data[TempMonitor.STATE_CURRENT_TEMP] = msg.temp_actual;
    TempMonitor.data[TempMonitor.STATE_MAX_TEMP] = msg.temp_maxima;
    TempMonitor.data[TempMonitor.STATE_MIN_TEMP] = msg.temp_minima;
    TempMonitor.data[TempMonitor.STATE_AVG_TEMP] = msg.temp_promedio;

    //ui.updateUI();

    // Blink on recieve
    PORTB ^= (1 << PB5);

    // Start task to send msg through Serial
    SystemClock.attach(Task(1, comm_send_msg));
}

void start_i2c() {
    // Set as slave or master
    Serial.println(TempMonitor.data[TempMonitor_::STATE_CURRENT_TEMP]);

    if(TempMonitor.data[TempMonitor_::STATE_CURRENT_TEMP] > 109) {
        I2CComunication.mode = I2CComunication.SLAVE;
        ui.i2c_mode = ui.SLAVE;
        Serial.println("I'm SLAVE");
        lm35.enabled = false;
        Wire.begin(I2CComunication.ADDRESS);
        Wire.onReceive(communication);
    } else {
        I2CComunication.mode = I2CComunication.MASTER;
        ui.i2c_mode = ui.MASTER;
        Serial.println("I'm MASTER");
        Wire.begin();
    }

    //can start the communication
    I2CComunication.comm_ready = true;
}

void setup() {

    //Inicializar el pin del led como salida
    DDRB |= (1 << DDB5);

    ui.serial_enabled = false;
    // Initialize lcd screen
    ui.initScreen();

    lm35.initialize();
    KeypadDriver.initialize();

    ADCManager.insertDriver(&lm35, 1);
    ADCManager.insertDriver(&KeypadDriver, 0);

    Serial.begin(115200);

    //ui.updateUI();

    // TODO: Reset welcome message
    //SystemClock.attach(Task(1, ui.disable_message_print));

    // Start I2C communication
    SystemClock.attach(Task(500, start_i2c));


    SystemClock.attach(Task(1000, ui.disable_message_print));
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
        ;//ui.updateUI();
}

