#ifndef ADCMANAGER_H_
#define ADCMANAGER_H_

#include "Arduino.h"
#include "Driver.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "SystemClock.h"


// ISR C++ mapping
extern "C" void ADC_vect(void) __attribute__ ((signal));

class ADCManager_ {
public:
    //constructor
    ADCManager_();
    //number of ADC entries
    static const uint8_t CANT_ADC = 6;
    //drivers for each ADC
    Driver drivers[CANT_ADC];
    //Insert driver
    void insertDriver(Driver driver, uint8_t adc);
private:
    //the routine to initialize the adc
    void adc_initializer();
    // ADC ISR
    friend void ADC_vect(void);
    //driver to execute
    uint8_t current;
    //disable ADC Interrupts
    void disable_adc_interrupt();
    //enable ADC Interrupts
    void enable_adc_interrupts();

    static void (*vref_setters[8])();
    static void vref_setter_common(int);
    static void vref_setter_ch0();
    static void vref_setter_ch1();
    static void vref_setter_ch2();
    static void vref_setter_ch3();
    static void vref_setter_ch4();
    static void vref_setter_ch5();
    static void vref_setter_ch6();
    static void vref_setter_ch7();
};

extern ADCManager_ ADCManager;

#endif /* ADCMANAGER_H_ */