/*************************
Name: Garrett Genord
Class: EGR226-903
Professor Ekin
Due Date: 12/9/20
Assignment: Embedded Systems Design: Semester Project
Description: This is the .h file for the output setting libary. The functions prototyped in this file are used to set different outputs of the control system (DC motor, servo motor,
             RBG module, LCD screensaver, etc).
 **************************/

#ifndef OUTPUTSET_LIB_H_
#define OUTPUTSET_LIB_H_

// The following functions that are prototyped all serve to set an output of the control system
void setDC(double dutyCycle); // Prototype for the function that will set the DC motor with the duty cycle that is passed to it
void setDoor(char doorState); // Prototype for the function that will either set the door state to open (servo open, green LED on) or to closed (servo closed, red LED on). The state is set based off the indicator passed to the function ('o': open, 'c': closeed)
void setRGBLEDs(char color, double dutyCycle); // Prototype for the function that will set the duty cycle of one of the LEDs on the RGB LED module. The desired color to set ('R', 'G', or 'B') is passed to it along with the desired duty cycle
void setDoorAlarm(int alarmOn); // Prototype for the function that will either turn the door alarm on or off based on the input passed to it (1: on, 0: off)
void LCDbrightnessSet(); // Prototype for the function that will read the ADC conversion value and set the duty cycle of the LCD brightness accordingly
void screenSaver(); // Prototype for the function that will continously display the scrolling smiley face screensaver until a press is read from the keypad


#endif /* OUTPUTSET_LIB_H_ */
