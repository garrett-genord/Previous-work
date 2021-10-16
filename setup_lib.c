/*************************
Name: Garrett Genord
Class: EGR226-903
Professor Ekin
Due Date: 12/9/20
Assignment: Embedded Systems Design: Semester Project
Description: This is the .c file for the setup libary. The functions in this file are used to setup different aspects of the control system (DC motor, servo motor,
             RBG module, LCD screensaver, etc). Let it be known that the LCD and SysTick initialization functions ARE in this library!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 **************************/

#include "msp.h" // MSP library
#include <stdio.h> // Standard libraries included for debugging by printing to console
#include <stdlib.h>
#include "setup_lib.h"

////////////////// Start of the setup/initialization functions
void setupDoorServo_LED(){ // This function will setup the servo and the feedback LEDs for the door
    // Setting up PWM TimerA output for servo
    P7SEL0 |= BIT6;
    P7SEL1 &= ~BIT6;
    P7DIR |= BIT6;

    // Setting up GPIO for door feedback LEDs
    P3SEL0 &= ~(BIT7 | BIT6);
    P3SEL1 &= ~(BIT7 | BIT6);
    P3DIR |= (BIT7 | BIT6);

    // Starting off w/ red LED on
    P3OUT &= ~BIT6; // Green LED off
    P3OUT |= BIT7; // Red LED on

    // Setting up TimerA1 for servo and DC motor
    TIMER_A1 -> CCR[0] = ((int)(((1.0/50.0) * 375000))); // Setting the period of the signal (in counts), which is the desired period (in seconds) multiplied by the clock frequency (which was divided by 8 for this program)
    TIMER_A1 -> CCTL[2] = TIMER_A_CCTLN_OUTMOD_7; // Setting reset/mode 7
    TIMER_A1 -> CTL = (0x0214 | BIT7 | BIT6); // Using SMCLK, up mode, dividing clock by 8, clear TAR to start

    // Starting servo door closed
    TIMER_A1 -> CCR[2] = ((int)(0.05 * (((1.0/50.0) * 375000))));
}

void setupDC(){ // This function will setup the DC motor pins

    P7SEL0 |= BIT4; // Setting up TimerA output
    P7SEL1 &= ~BIT4;
    P7DIR |= BIT4;

    TIMER_A1 -> CCTL[4] = TIMER_A_CCTLN_OUTMOD_7; // Setting reset/mode 7
    TIMER_A1 -> CCR[4] = 0; // Starting the motor initially off
}

void setupLED_PWM(){ // This function will setup the TimerA PWM output for the RBG LEDs and the LCD brightness
    TIMER_A2 -> CCR[0] = ((int)(((1.0/250.0) * 375000))); // Setting the period of the signal (in counts), which is the desired period (in seconds) multiplied by the clock frequency (which was divided by 8 for this program)
    TIMER_A2 -> CCTL[1] = TIMER_A_CCTLN_OUTMOD_7; // Setting reset/mode 7
    TIMER_A2 -> CCTL[2] = TIMER_A_CCTLN_OUTMOD_7; // Setting reset/mode 7
    TIMER_A2 -> CCTL[3] = TIMER_A_CCTLN_OUTMOD_7; // Setting reset/mode 7
    TIMER_A2 -> CCTL[4] = TIMER_A_CCTLN_OUTMOD_7; // Setting reset/mode 7

    TIMER_A2 -> CTL = (0x0214 | BIT7 | BIT6); // Using SMCLK, up mode, dividing clock by 8, clear TAR to start

    P5SEL0 |= (BIT6 | BIT7); // Setting TimerA output for TA2.1-TA2.4
    P5SEL1 &= ~(BIT6 | BIT7);
    P5DIR |= (BIT6 | BIT7);

    P6SEL0 |= (BIT6 | BIT7);
    P6SEL1 &= ~(BIT6 | BIT7);
    P6DIR |= (BIT6 | BIT7);


    TIMER_A2 -> CCR[1] = 0; // Initializing everything to 0
    TIMER_A2 -> CCR[2] = 0;
    TIMER_A2 -> CCR[3] = 0;
    TIMER_A2 -> CCR[4] = 0;

}

void LCD_adcSetup(void) // This function will setup an ADC conversion on P5.5 for the LCD brightness
{
    ADC14->CTL0 = 0x00000010; //power on and disabled during configuration
    ADC14->CTL0 |= 0x04D80300; // S/H pulse mode, MCLCK, 32 sample clocks, sw trigger, /4 clock divide
    ADC14->CTL1 = 0x00000030; // 14-bit resolution
    ADC14->MCTL[5] = 0; // A0 input, single ended, vref=avcc
    P5->SEL1 |= 0x20; // configure pin 5.5 for AO
    P5->SEL0 |= 0x20;
    ADC14->CTL1 |= 0x00050000; //start converting at mem reg 5
    ADC14->CTL0 |= 2; //enable ADC after configuration
}

void keypad_init(){ // This function will initialize GPIO for the keypad pins

    P2SEL0 = 0; // GPIO for all the input pins on the keypad
    P2SEL1 = 0;

    P2DIR = 0; // Setting all pins/rows/columns to inputs

    P2REN |= 0x0F; // Giving all of the row input pins pull-up resistors
    P2OUT |= 0x0F;
}

void screenSaverIntSetup(){ // This function will set up the 10 second Timer32 interrupt for the screensaver
    NVIC -> ISER[0] |= (1 << 25); // Setting interrupt vector

    TIMER32_1 -> CONTROL = 0xE2; // Timer enabled, periodic mode, interrupt enabled, no clk divide, 32 bit counter, wrapping mode
    TIMER32_1 -> LOAD = 30000000; // Setting the load and background load
    TIMER32_1 -> BGLOAD = 30000000;
}

void setupAlarm(){ // This function will set up the TimerA PWM output for the Piezzo buzzer
    P8SEL0 |= BIT2;
    P8SEL1 &= ~BIT2;
    P8DIR |= BIT2;

    TIMER_A3 -> CCR[0] = ((int)(((1.0/3000.0) * 375000))); // Setting the period of the signal (in counts), which is the desired period (in seconds) multiplied by the clock frequency (which was divided by 8 for this program)
    TIMER_A3 -> CCTL[2] = TIMER_A_CCTLN_OUTMOD_7; // Setting reset/mode 7 // SHOULD BE cctl2 BC TA1.2?
    TIMER_A3 -> CTL = (0x0214 | BIT7 | BIT6); // Using SMCLK, up mode, dividing clock by 8, clear TAR to start

    TIMER_A3 -> CCR[2] = 0; // Initialize to 0
}

void setupIntButtons(){ // This function will set up the GPIO interrupts for the buttons on P5
    NVIC -> ISER[1] = 1 << (39 - 32); // Setting interrupt vector

    P5SEL0 &= ~(BIT0 | BIT2); // GPIO for the pins
    P5SEL1 &= ~(BIT0 | BIT2);

    P5DIR &= ~(BIT0 | BIT2); // Setting the pins as inputs with pull-down resistors
    P5REN |= (BIT0 | BIT2);
    P5OUT &= ~(BIT0 | BIT2);

    P5IES &= ~(BIT0 | BIT2); // Set the interrupt to trigger when moving from low to high
    P5IFG = 0; // Clearing interrupt register
    P5IE |= (BIT0 | BIT2); // Enabling the interrupts
}

void LCD_Init(){ // This function will run the sequence that was specified in the lab handout to initialize the LCD

    P4SEL0 = 0; // Setting up GPIO for port 4
    P4SEL1 = 0;
    P4DIR = ~0; // All port 4 pins are outputs

    // Instead of using the commandWrite function to call the commands with 4 bit (or less) numbers, the pushNibble function was used
    P4OUT &= ~BIT1;
    pushNibble(3);
    delay_ms(100);
    pushNibble(3);
    delay_micro(200);
    pushNibble(3);
    delay_ms(100);
    pushNibble(2);

    delay_micro(100);
    commandWrite(0x28);
    delay_micro(100);

    delay_micro(100);
    commandWrite(0x0F);
    delay_micro(100);
    commandWrite(0x01);
    delay_micro(100);
    commandWrite(0x06);
    delay_ms(10);
}

void SysTick_Init(){ // This function will properly initialize the SysTick timer without interrupts
    SysTick -> CTRL = 0; // Disable SysTick during this process
    SysTick -> LOAD = 0x00FFFFFF; // Setting the max reload value
    SysTick -> VAL = 0; // Any write to the current value register is cleared
    SysTick -> CTRL = 0x00000005; // Enabling SysTick, 3MHz, no interrupts
}
////////////////// End of the setup/initialization functions
