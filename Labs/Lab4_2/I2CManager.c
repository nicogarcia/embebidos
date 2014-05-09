#include "Wire.h"

int static const  MASTER 0
int static const SLAVE 1

class I2CManager {
public:
    //contructor depends the mode
    I2CManager(uint8_t mod);
    //comunication with the others arduinos
    void communication();


private:
    //Device's mode (master or slave)
    uint8_t mode;

};