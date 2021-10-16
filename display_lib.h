/*************************
Name: Garrett Genord
Class: EGR226-903
Professor Ekin
Due Date: 12/9/20
Assignment: Embedded Systems Design: Semester Project
Description: This is the .h file for the display libary. The functions prototyped in this file are used to display different menus/screens on the LCD
 **************************/

#ifndef DISPLAY_LIB_H_
#define DISPLAY_LIB_H_

// The following functions that are prototyped all serve to display different menus on the LCD
void displayMainMenu(); // Prototype for the function that displays the main menu on the LCD
void displayDoorArmedMenu(); // Prototype for the function that displays the door menu with the door armed
void displayDoorUnarmedMenu(); // Prototype for the function that displays the door menu with the door unarmed
void displayInvalidPIN(); // Prototype for the function that will display a message informing the user that the pin they entered to unarm the door is invalid and that the door is still armed
void displayValidPIN(); // Prototype for the function that will display a message informing the user that the pin they entered to unarm the door is valid and that the door is unarmed
void displayMotorMenu(); // Prototype for the function that will display the motor menu
void displayLightsMenu(); // Prototype for the function that will display the first lights menu (allows user to choose between R, G, & B)
void displayLightsMenu_2(char color); // Prototype for the function that will display the second lights menu for either R, G, or B depending on which char indicator is passed to it ('R', 'G', or 'B')

#endif /* DISPLAY_LIB_H_ */
