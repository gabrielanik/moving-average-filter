#include <stdint.h>
#include <stdbool.h>
#include <math.h>

// Texas Instruments TivaWare libraries:
// hardware definitions and drivers for system control,
// ADC, GPIO, and timer peripherals of the TM4C123G microcontroller.
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"

// Preprocessor macros
#define BUFFER_SIZE 1000
#define SAMPLERATE 44000

/*
 * Since we use 8 LEDs (8 inputs are queried sequentially), the
 * maximum ADC value is:
 *
 * maxADCValue = 2^8 - 1 = 255
 *
 * However, we are required to square the ADC values first
 * and then store them in the ring buffer.
 * Therefore:
 *
 * maxADCValue = 255^2 = 65025
 *
 */
#define maxADCValue 65025

// Function declarations
void adcIntHandler(void);
void setup(void);

// Global variables

// Ring buffer
int32_t ringBuffer[BUFFER_SIZE];

// Variable used to increment the ring buffer index
int32_t i = 0;

// Sum of the squared ADC values
double sum = 0;

void main(void){
    setup();
    while(1){}
}

void setup(void){ // Configures the microcontroller

    // Configure system clock
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    uint32_t period = SysCtlClockGet()/SAMPLERATE;

    // Enable peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    // Configure GPIO
    GPIOPinTypeADC(GPIO_PORTE_BASE,GPIO_PIN_2);

    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|
                          GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

    // Configure timer
    TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, period - 1);
    TimerControlTrigger(TIMER0_BASE,TIMER_A,true);
    TimerEnable(TIMER0_BASE,TIMER_A);

    // Configure ADC
    ADCClockConfigSet(ADC0_BASE,ADC_CLOCK_RATE_FULL,1);
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_TIMER, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH1|ADC_CTL_IE|ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
    ADCIntClear(ADC0_BASE,3);
    ADCIntRegister(ADC0_BASE,3,adcIntHandler);
    ADCIntEnable(ADC0_BASE,3);
}

void adcIntHandler (void){

    uint32_t adcInputValue;
    ADCSequenceDataGet(ADC0_BASE,3,&adcInputValue);

    // Define a variable to store the squared ADC value
    double valueFromMicro = pow((double)adcInputValue, 2);

    /*
     * Define a variable for the ring buffer index.
     * The modulo operation ensures that the index is reset to zero
     * once the ring buffer becomes full.
     *
     */
    int32_t newIndex = i % BUFFER_SIZE;

    /*
     * Store the old value so that it can be subtracted
     * from the sum later.
     *
     */
    int32_t oldValue = ringBuffer[newIndex];

    // Calculate the moving average using the squared ADC values
    sum += (- oldValue + valueFromMicro)/BUFFER_SIZE;

    // Store the new value in the ring buffer
    ringBuffer[newIndex] = valueFromMicro;

    // Increment the index
    i++;

    // Map the output of the moving average filter to 8 levels
    if (sum <= 100){
        GPIOPinWrite(GPIO_PORTB_BASE,
        GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, 0x00);

    } else if (sum <= maxADCValue / 8 ) {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, 0xFF);
        GPIOPinWrite(GPIO_PORTB_BASE,
        GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, 0x00);

    } else if (sum <= 2 * maxADCValue / 8 ) {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0xFF);
        GPIOPinWrite(GPIO_PORTB_BASE,
        GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5, 0x00);

    } else if (sum <= 3 * maxADCValue / 8 ) {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0xFF);
        GPIOPinWrite(GPIO_PORTB_BASE,
        GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4, 0x00);

    } else if (sum <= 4 * maxADCValue / 8 ) {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0xFF);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,
        0x00);

    } else if (sum <= 5 * maxADCValue / 8 ) {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0xFF);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2, 0x00);

    } else if (sum <= 6 * maxADCValue / 8 ) {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0xFF);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1, 0x00);

    } else if (sum <= 7 * maxADCValue / 8 ) {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0xFF);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0x00);

    } else if (sum <= maxADCValue ) {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0xFF);
    }

    ADCIntClear(ADC0_BASE,3);
}