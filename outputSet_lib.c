/*************************
Name: Garrett Genord
Class: EGR226-903
Professor Ekin
Due Date: 12/9/20
Assignment: Embedded Systems Design: Semester Project
Description: This is the .c file for the output setting libary. The functions in this file are used to set different outputs of the control system (DC motor, servo motor,
             RBG module, LCD screensaver, etc).
 **************************/

#include "msp.h" // MSP library
#include <stdio.h> // Standard libraries included for debugging by printing to console
#include <stdlib.h>
#include <string.h> // String library for printing to LCD
#include <math.h> // Math library included if needed
#include "display_lib.h" // Including library to display different menus on LCD
#include "setup_lib.h" // Including library to setup different aspects of the control system
#include "outputSet_lib.h" // Including library to set different outputs of the control system
#include "LCDoperate_lib.h" // Including library for LCD operating functions
#include "SysTickOperate_lib.h" // Including library for SysTick operating function


/////////////////// Start of output control functions
void setDC(double dutyCycle){ // This function will set the duty cycle of the TimerA pin connected to the DC motor
    TIMER_A1 -> CCR[4] = ((int)( ( dutyCycle * ((1.0/50.0) * 375000) )));
}

void setDoor(char doorState){ // This function will open or close the door and illuminate the appropriate light based off the indicator passed to it
    if(doorState == 'o'){ // Opening door
        TIMER_A1 -> CCR[2] = ((int)(0.1 * (((1.0/50.0) * 375000) - 1))); // Opening servo
        P3OUT &= ~BIT7; // Red LED off
        P3OUT |= BIT6; // Green LED on
    }

    if(doorState == 'c'){ // Closing door
        TIMER_A1 -> CCR[2] = ((int)(0.05 * (((1.0/50.0) * 375000) - 1))); // Closing servo
        P3OUT &= ~BIT6; // Green LED off
        P3OUT |= BIT7; // Red LED on
    }
}

void  setRGBLEDs(char color, double dutyCycle){ // This function will set the brightness of the LEDs on the RBG module based off the indicator passed to it
    if(color == 'R')
        TIMER_A2 -> CCR[1] = ((int)((dutyCycle * (1.0/250.0) * 375000)));

    if(color == 'G')
        TIMER_A2 -> CCR[2] = ((int)((dutyCycle * (1.0/250.0) * 375000)));

    if(color == 'B')
        TIMER_A2 -> CCR[3] = ((int)((dutyCycle * (1.0/250.0) * 375000)));

}

void setDoorAlarm(int alarmOn){ // This function will turn the Piezzo buzzer alarm on or off based on the flag passed to it
    if(alarmOn) TIMER_A3 -> CCR[2] = ((int)(( 0.30 * (1.0/3000.0) * 375000)));
    else TIMER_A3 -> CCR[2] = 0;
}

void LCDbrightnessSet(){ // This function will convert the voltage value on P5.5 with the ADC and set the TimerA PWM output for the LCD brightness accordingly
    static double voltage = 0;
    static double dutyCycle = 0;
    static int result = 0;

    ADC14->CTL0 |= 1; // Starting a conversion
    while(!ADC14->IFGR0); // Waiting until the conversion is complete
    result = ADC14->MEM[5]; // Storing the conversion value
    voltage = (result * (3.3/pow(2,14))); // Converting the raw sample to a voltage value

    dutyCycle = voltage/3.3; // Calculating the duty cycle based on Vref
    if(dutyCycle < 0.1) dutyCycle = 0; // If the duty cycle is less than 0.1, set the output to 0 to avoid a flickering screen
    TIMER_A2 -> CCR[4] = ((int)( dutyCycle * ((1.0/250.0) * 375000))); // Setting PWM output

}

void screenSaver(){ // This function will display the scrolling smiley face screen saver
    commandWrite(1); // Clearing display and delaying
    delay_ms(10);

    int lineCount = 1; // Variable to store the current line
    int i; // For-loop counter

    while(1){
        if(lineCount == 1){ // First line
            commandWrite(1); // Clearing display and delaying
            delay_ms(10);
            commandWrite(0x80); // start of first line
            delay_ms(10);
            printStringToLCD(":)"); // Printing smiley face

            for(i = 0; i < 13; i++){
                commandWrite(0b00011100); // Shifting display every 250 ms
                delay_ms(250);
                if(Read_Keypad()){ // If a keypad press is detected, reset the 10 second countdown and exit the function
                    TIMER32_1 -> LOAD = 30000000;
                    return;
                }
            }
            lineCount++; // Increment the line
        } else if (lineCount == 2) { // Second line
            commandWrite(1); // Clearing display and delaying
            delay_ms(10);
            commandWrite(0xC0); // start of second line
            delay_ms(10);
            printStringToLCD(":)");
            for(i = 0; i < 13; i++){ // Printing smiley face
                commandWrite(0b00011100); // Shifting display every 250 ms
                delay_ms(250);
                if(Read_Keypad()){ // If a keypad press is detected, reset the 10 second countdown and exit the function
                    TIMER32_1 -> LOAD = 30000000;
                    return;
                }
            }

            lineCount++; // Increment the line
        } else if (lineCount == 3){ // Third line
            commandWrite(1); // Clearing display and delaying
            delay_ms(10);
            commandWrite(0x90); // start of third line
            delay_ms(10);
            printStringToLCD(":)"); // Printing smiley face
            for(i = 0; i < 13; i++){
                commandWrite(0b00011100); // Shifting display every 250 ms
                delay_ms(250);
                if(Read_Keypad()){ // If a keypad press is detected, reset the 10 second countdown and exit the function
                    TIMER32_1 -> LOAD = 30000000;
                    return;
                }
            }

            lineCount++; // Increment the line
        } else if (lineCount == 4){ // Line 4
            commandWrite(1); // Clearing display and delaying
            delay_ms(10);
            commandWrite(0xD0); // start of fourth line
            delay_ms(10);
            printStringToLCD(":)"); // Printing smiley face
            for(i = 0; i < 13; i++){
                commandWrite(0b00011100); // Shifting display every 250 ms
                delay_ms(250);
                if(Read_Keypad()){ // If a keypad press is detected, reset the 10 second countdown and exit the function
                    TIMER32_1 -> LOAD = 30000000;
                    return;
                }
            }

            lineCount = 1; // Reset the line
        }

        if(Read_Keypad()){ // If a keypad press is detected, reset the 10 second countdown and exit the function
            TIMER32_1 -> LOAD = 30000000;
            return;
        }
    }

}


/////////////////// End of output control functions

