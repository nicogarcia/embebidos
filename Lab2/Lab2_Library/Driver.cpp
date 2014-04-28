#include "Driver.h"

typedef void (*fptr)();

Driver::Driver () {
    time = 0;
    function = NULL;
    last_execution = 0;
    enable = false;
}

Driver::Driver(fptr funct, unsigned long every_time) {
    time = every_time;
    function = funct;
    last_execution = 0;
    enable = true;
}

void Driver::execute_function() {
    if (SystemClock.getMillis() - last_execution >= time) {
        last_execution = SystemClock.getMillis();
        function();
    }
}

