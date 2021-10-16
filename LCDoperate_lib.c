/*************************
Name: Garrett Genord
Class: EGR226-903
Professor Ekin
Due Date: 12/9/20
Assignment: Embedded Systems Design: Semester Project
Description: This is the .c file for the LCD operation libary. The functions in this file are used to operate the LCD.
             Let it be known that the LCD initialization function is NOT in this library !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 **************************/

#include "msp.h" // MSP library
#include <stdio.h> // Standard libraries included for debugging by printing to console
#include <stdlib.h>
#include <string.h> // String library for printing to LCD
#include "LCDoperate_lib.h"

////////////////// Start of LCD operating functions
void PulseEnablePin(){ // This function will pulse the enable pin on the LCD for 10 microseconds
    P4OUT &= ~BIT2; // Setting en pin low
    delay_micro(10); // Delay 10 us
    P4OUT |= BIT2; // Setting it high
    delay_micro(10); // Delay 10 us
    P4OUT &= ~BIT2; // Setting it low
    delay_micro(10); // Delay 10 us
}

void pushNibble(uint8_t nibble){ // This function will take the nibble data passed to it, parse it for the 4 least significant bits, send them to the D pins on the LCD, and then pulse the enable pin so the LCD accepts the inputs
    P4OUT &= ~0xF0; // Making the last 4 bits of port 4 zeros
    P4OUT |= ((nibble & 0x0F) << 4); // Making sure the most significant bits of the nibble byte are 0, shifting the important least significant bits over, and sending them to the D pins on the LCD
    PulseEnablePin(); // Pulsing the enable pin so the LCD will accept the data
}

void pushByte (uint8_t byte){ // This function will take the "byte" passed to it, parse it into two nibbles, and send them both through the pushNibble function so they can be pushed to the LCD
    uint8_t nibble;

    nibble = ((byte & 0xF0) >> 4); // Taking the 4 most significant bits and pushing them as a nibble
    pushNibble(nibble);
    nibble = byte & 0x0F; // Taking the 4 least significant bits and pushing them as a nibble
    pushNibble(nibble);
    delay_micro(100);
}

void commandWrite(uint8_t command){ // This function will first set the RS pin to 0 to signify a command, then it will take the command passed to it and have it pushed to the LCD to be executed
    P4OUT &= ~BIT1;
    pushByte(command);
}

void dataWrite(uint8_t data){ // This function will first set the RS pin to 1 to signify data, then it will take the data passed to it and have it pushed to the LCD to be printed
    P4OUT |= BIT1;
    pushByte(data);
}

void printStringToLCD(char string[]){ // This function will take the string that is passed to it and print it to the LCD
    int i;

    for(i = 0; i < strlen(string); i++){ // Iterating through the character array and printing each character to the LCD
        dataWrite(string[i]);
    }
}
/////////////////// End of LCD operating functions


