/*************************
Name: Garrett Genord
Class: EGR226-903
Professor Ekin
Due Date: 12/9/20
Assignment: Embedded Systems Design: Semester Project
Description: This is the .h file for the LCD operation libary. The functions prototyped in this file are used to operate the LCD.
             Let it be known that the LCD initialization function is NOT in this library!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 **************************/

#ifndef LCDOPERATE_LIB_H_
#define LCDOPERATE_LIB_H_

// The following functions that are prototyped all serve to operate the LCD
void PulseEnablePin(); // Prototype for the function that will pulse the enable pin on the LCD for 10 microseconds
void pushNibble(uint8_t nibble); // Prototype for the function that will take the "nibble" passed to it and push it to the LCD
void pushByte(uint8_t byte); // Prototype for the function that will take the "byte" passed to it, parse it into two nibbles, and send them both through the pushNibble function so they can be pushed to the LCD
void commandWrite(uint8_t command); // Prototype for the function that will take the "command" passed to it and have it pushed to the LCD to be executed
void dataWrite(uint8_t data); // Prototype for the function that will take the "data" character passed to it and have it pushed to the LCD to be printed
void printStringToLCD(char string[]); // Prototype for the function that prints a string to the LCD


#endif /* LCDOPERATE_LIB_H_ */
