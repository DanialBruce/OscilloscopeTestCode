/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include <chrono>
#include <cmath>
#include <ctime>


// Blinking rate in milliseconds
#define BLINKING_RATE     500ms
/* #define pwm_myperiod  4.0
#define pwm_pulsewidth 2 */
//Threads
Thread ptmRead_thread;

Thread ptmReadPrint_thread;


//pins
AnalogIn pinA3(A3);
AnalogIn pinA4(A4);

PwmOut pwm_pin(A2);


//func prototypes
void ptmReading();
void ptmReading_print();

//vars
const float pwm_period =  4.0;
//const float pwm_pulsewidth = 2;

float A3value;
float A4value;

float& period_X = A4value; // multiplier value for PWM period
float& dutyCycle_X = A3value;   //use directly, or as an multiplier for DutyCycle Constant
//float& pulseWidth_X;
int main()
{
    // Initialise the digital pin LED1 as an output
    DigitalOut led(LED1);
    // specify period first
    pwm_pin.period(4.2f);      // 4 second period
    //pwm_pin.write(0.50f);      // 50% duty cycle, relative to period
    //pwm_pin = 0.5f;          // shorthand for led.write()
    //pwm_pin.pulsewidth(2);   // alternative to led.write, set duty cycle time in seconds
    
    ptmRead_thread.start(ptmReading);
    ptmReadPrint_thread.start(ptmReading_print);

    pinA3.set_reference_voltage(3.3);
    pinA4.set_reference_voltage(3.3);
    ThisThread::sleep_for(1s);
    
    while (true) {
        pwm_pin.period(pwm_period * period_X);
        pwm_pin.write(dutyCycle_X);
        led = !led;
        ThisThread::sleep_for(10s);
    }
}


void ptmReading(){
    while (true) {
        A3value = pinA3.read();
        A4value = pinA4.read();
    }
}


void ptmReading_print(){

    while (true) {
        printf("pin A3 (raw): %f\033[K\n", A3value);
        printf("pin A4 (raw): %f\033[K\n", A4value);
        printf("pin A3 (.2f): %.2f\033[K\n", A3value);
        printf("pin A4 (.2f): %.2f\033[K\n", A4value);
        printf("Voltage on A3: %.2f\033[K\n", pinA3.read_voltage());
        printf("Voltage on A4: %.2f\033[K\n", pinA4.read_voltage());
        printf("\033[H");
        ThisThread::sleep_for(500ms);
    }
}