/*************************
Name: Garrett Genord
Class: EGR226-903
Professor Ekin
Due Date: 12/9/20
Assignment: Embedded Systems Design: Semester Project
Description: This is the .c file for the display libary. The functions in this file are used to display different menus/screens on the LCD
 **************************/

#include "msp.h" // MSP library
#include <stdio.h> // Standard libraries included for debugging by printing to console
#include <stdlib.h>
#include <string.h> // String library for printing to LCD

/////////////////// Start of display functions
void displayMainMenu(){ // This function will display the main menu on the LCD
    commandWrite(1); // Clearing display
    delay_ms(10); // Waiting to avoid propagation display issues

    printStringToLCD("   Main Menu"); // Printing 1st line

    delay_ms(10);
    commandWrite(0b11000000); // Moving to next line
    printStringToLCD("(1) Door");

    delay_ms(10);
    commandWrite(0b10010000); // Moving to next line
    printStringToLCD("(2) Motor");

    delay_ms(10);
    commandWrite(0b11010000); // Moving to next line
    printStringToLCD("(3) Lights");

}

void displayDoorArmedMenu(){ // This function will display the door armed menu
    commandWrite(1); // Clearing display
    delay_ms(10); // Waiting to avoid propagation display issues

    printStringToLCD("   Door Menu");

    delay_ms(10);
    commandWrite(0b11000000); // Moving to next line
    printStringToLCD("(1) Open");

    delay_ms(10);
    commandWrite(0b10010000); // Moving to next line
    printStringToLCD("(2) Close");


    delay_ms(10);
    commandWrite(0b11010000); // Moving to next line
    printStringToLCD("(3) Unarm");
}

void displayDoorUnarmedMenu(){ // This function will display the door unarmed menu
    commandWrite(1); // Clearing display
    delay_ms(10); // Waiting to avoid propagation display issues

    printStringToLCD("   Door Menu");

    delay_ms(10);
    commandWrite(0b11000000); // Moving to next line
    printStringToLCD("(1) Open");

    delay_ms(10);
    commandWrite(0b10010000); // Moving to next line
    printStringToLCD("(2) Close");


    delay_ms(10);
    commandWrite(0b11010000); // Moving to next line
    printStringToLCD("(3) Arm");
}

void displayInvalidPIN(){ // This function will display an invalid PIN message to inform the user that the door is still armed
    commandWrite(1); // Clearing display
    delay_ms(10); // Waiting to avoid propagation display issues

    printStringToLCD("   Invalid PIN");

    delay_ms(10);
    commandWrite(0b11000000); // Moving to next line
    printStringToLCD("Door is armed");
}

void displayValidPIN(){ // This function will display a valid PIN message to inform the user that the door is unarmed
    commandWrite(1); // Clearing display
    delay_ms(10); // Waiting to avoid propagation display issues

    printStringToLCD("   Valid PIN");

    delay_ms(10);
    commandWrite(0b11000000); // Moving to next line
    printStringToLCD("Door is unarmed");
}

void displayMotorMenu(){ // This function will display the motor menu
    commandWrite(1); // Clearing display
    delay_ms(10); // Waiting to avoid propagation display issues

    printStringToLCD("   Motor Menu");
    delay_ms(10);
    commandWrite(0b11000000); // Moving to next line
    printStringToLCD("Enter speed");

    delay_ms(10);
    commandWrite(0b10010000); // Moving to next line
    printStringToLCD("(0-9)");
}

void displayLightsMenu(){ // This function will display the first lights menu
    commandWrite(1); // Clearing display
    delay_ms(10); // Waiting to avoid propagation display issues

    printStringToLCD("   Lights Menu");
    delay_ms(10);
    commandWrite(0b11000000); // Moving to next line
    printStringToLCD("Choose color");

    delay_ms(10);
    commandWrite(0b10010000); // Moving to next line
    printStringToLCD("R:1 G:2 B:3");
}

void displayLightsMenu_2(char color){ // This function will display the second lights menu for either the red, green, or blue LED based off the color indicator passed to it
    commandWrite(1); // Clearing display
    delay_ms(10); // Waiting to avoid propagation display issues

    if(color == 'R'){
        printStringToLCD("   Red Light");
    }

    if(color == 'G'){
        printStringToLCD("   Green Light");
    }

    if(color == 'B'){
        printStringToLCD("   Blue Light");
    }

    delay_ms(10);
    commandWrite(0b11000000);
    printStringToLCD("Set brightness");

    delay_ms(10);
    commandWrite(0b10010000);
    printStringToLCD("(0-100)");
}
/////////////////// End of display functions
