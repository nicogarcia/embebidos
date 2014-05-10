#include "I2CComunication.h"
#include "Wire.h"

I2CComunication_ I2CComunication;

I2CComunication_::I2CComunication_() {
    mode = MASTER;
    comm_ready = false;
}

void I2CComunication_::I2CSendMessage(TempMessage message) {
    double temps[4];

    // Put temps in array
    temps[0] = message.temp_actual;
    temps[1] = message.temp_maxima;
    temps[2] = message.temp_minima;
    temps[3] = message.temp_promedio;

    // Write mode
    writing_msg[0] = (byte)(message.mode >> 8);
    writing_msg[1] = (byte)(message.mode);

    for(int i = 1; i < 5; i++) {
        writing_msg[2 * i] = (byte)(temps[i - 1] + NUMBER_PADDING);
        writing_msg[2 * i + 1] = (byte)((byte)(temps[i - 1] * 100) % 100 + NUMBER_PADDING);
    }

    // Write start token
    Wire.beginTransmission(0);
    Wire.write(START_TOKEN);

    // Write message
    for(int i = 0; i < MSG_LENGTH; i++)
        Wire.write(writing_msg[i]);

    // Write end token
    Wire.write(END_TOKEN);

    Wire.endTransmission();
}

TempMessage I2CComunication_::I2CReadMessage() {
    int msg_pointer = 0;

    while(Wire.available()) {

        byte current_byte;

        // Read Byte
        current_byte = Wire.read();

        // Start token read
        if(current_byte == START_TOKEN) {
            msg_pointer = 0;
            continue;
        }

        // End token read
        if(current_byte == END_TOKEN) {
            // Additional check, is pointer at the end?
            if(msg_pointer == MSG_LENGTH)
                return ParseMessage();
            continue;
        }

        // Number read
        reading_msg[msg_pointer++] = current_byte;
    }
    return TempMessage();
}

TempMessage I2CComunication_::ParseMessage() {
    TempMessage temp_msg;

    // Parse mode
    temp_msg.mode = (reading_msg[0] << 8) | reading_msg[1];

    // Make 2-Byte to Double precision conversion
    double temps[4];
    for(int i = 1; i < 5; i++) {
        temps[i - 1] = reading_msg[2 * i] - NUMBER_PADDING;
        temps[i - 1] += (reading_msg[2 * i + 1] - NUMBER_PADDING) / 100.0;
    }

    // Build struct to be returned
    temp_msg.temp_actual = temps[0];
    temp_msg.temp_maxima = temps[1];
    temp_msg.temp_minima = temps[2];
    temp_msg.temp_promedio = temps[3];

    return temp_msg;
}



