#include "ADCManager.h"
#include "Arduino.h"

ADCManager_ ADCManager;

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

    ADCSRA |= 1<<ADSC;	// Start Conversion
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

// Key ISR
void ADC_vect() {
    Driver *current_driver = &ADCManager.drivers[ADCManager.current];

    if (current_driver->enable && (SystemClock.getMillis() - current_driver->last_execution >= current_driver->time )) {

        current_driver->driver_ISR(ADC);

        current_driver->last_execution = SystemClock.getMillis();

    }
    ADCManager.current = (ADCManager.current + 1) % ADCManager.CANT_ADC;

    ADMUX = (1<< REFS1) | (1<<REFS0)  | (ADCManager.current & 0x07) ;

    ADCSRA |= 1<<ADSC;	// Start new ADC conversion
}