#include "ADCManager.h"
#include "util/delay.h"
#include "Arduino.h"

ADCManager_ ADCManager;
void (*ADCManager_::vref_setters[8])() = {
    vref_setter_ch0,
    vref_setter_ch1,
    vref_setter_ch2,
    vref_setter_ch3,
    vref_setter_ch4,
    vref_setter_ch5,
    vref_setter_ch6,
    vref_setter_ch7
};

ADCManager_::ADCManager_() {
    for(uint8_t i = 0; i < CANT_ADC; i++)
        drivers[i] = Driver();
    current = 0;
    adc_initializer();
}

void ADCManager_::insertDriver( Driver driver, uint8_t adc ) {
    if (adc < CANT_ADC) {
        //disable_adc_interrupt();
        noInterrupts();
        drivers[adc] = driver;
        SystemClock.attach(Task(driver.time, vref_setters[adc]));
        //enable_adc_interrupts();
        interrupts();
    }
}

// ADC initialization routine
void ADCManager_::adc_initializer() {
    // Based on example found in http://maxembedded.com/2011/06/20/the-adc-of-the-avr/
    noInterrupts();

    // AREF = AVcc
    //1 shift 6 places (01000000) to define the reference for the ADC. In this case 5V
    //ADLAR bit is set 0 in ADMUX register because we want the high part of the conversion
    // in ADCH register.
    ADMUX = (1<< REFS1) | (1<<REFS0) | (current & 0x07) ;

    // ADC Enable and prescaler of 128
    // 16000000/128 = 125000
    /**
     * ADEN: ADC Enable set to 1 to enable de ADC
     * ADIE: ADC Interrupt Enable. Set to 1 to enable de ADC interrupts
     * ADPS2::0: The prescalar is define by this three bits. 111 means 128.
     **/
    ADCSRA = (1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);

    interrupts();
}

void ADCManager_::disable_adc_interrupt() {
    noInterrupts();

    ADCSRA &= ~(1 << ADIE);

    interrupts();
}

void ADCManager_::enable_adc_interrupts() {
    noInterrupts();

    ADCSRA|=(1<<ADIE);

    interrupts();
}

volatile int next_channel = -1;
volatile bool adc_running = false;

// Key ISR
void ADC_vect() {
    if(next_channel == -1)
        return;

    Driver *current_driver = &ADCManager.drivers[next_channel];

    if(current_driver->enabled)
        current_driver->driver_ISR(ADC);

    next_channel = -1;
    adc_running = false;
}

void ADCManager_::vref_setter_common(int channel) {

}

int count = 0;
void ADCManager_::vref_setter_ch0() {
    int channel = 0;
    Driver* current_driver = &ADCManager.drivers[channel];

    if(!adc_running) {
        next_channel = channel;
        ADMUX = (1 << REFS0) | (channel & 0x07);

        adc_running = true;
        ADCSRA |= (1 << ADSC);
        count = 0;
    }

    if(count <= 1) {
        SystemClock.attach(Task(current_driver->time, vref_setter_ch0));
        count++;
    }
}

void ADCManager_::vref_setter_ch1() {
    int channel = 1;
    Driver* current_driver = &ADCManager.drivers[channel];

    while(adc_running);

    next_channel = channel;
    ADMUX = (1 << REFS1) | (1 << REFS0) | (channel & 0x07);

    _delay_ms(8);
    adc_running = true;
    ADCSRA |= (1 << ADSC);

    SystemClock.attach(Task(current_driver->time, vref_setter_ch1));
}

void ADCManager_::vref_setter_ch2() {

}

void ADCManager_::vref_setter_ch3() {

}

void ADCManager_::vref_setter_ch4() {

}

void ADCManager_::vref_setter_ch5() {

}

void ADCManager_::vref_setter_ch6() {

}

void ADCManager_::vref_setter_ch7() {

}
