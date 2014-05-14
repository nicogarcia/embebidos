#ifndef I2CMANAGER_H_
#define I2CMANAGER_H_

#include "Wire.h"
#include "CommProtocol.h"
#include "Arduino.h"

class I2CComunication_ {
public:

    //Constructor
    I2CComunication_();

    // Slaves addresses
    static const byte ADDRESSES[];

    //Device's mode (master or slave)
    uint8_t mode;

    TempMessage ParseMessage();

    void I2CSendMessage(TempMessage temps);

    TempMessage I2CReadMessage();

    bool comm_ready;

    int static const MASTER = 1;
    int static const SLAVE = 2;

    static const byte ADDRESS = 4;

    // Message length without tokens
    static const int MSG_LENGTH = 10;

    // Start and end tokens
    static const byte START_TOKEN = 110;
    static const byte END_TOKEN = 120;


private:

    // Number padding to map, for ex. 0 => 100, 1 => 101
    // This is to avoid getting false numbers, like 0
    static const byte NUMBER_PADDING = 0;

    byte writing_msg[MSG_LENGTH];
    byte reading_msg[MSG_LENGTH];

};
extern I2CComunication_ I2CComunication;


#endif /* I2CMANAGER_H_ */