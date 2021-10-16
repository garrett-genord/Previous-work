/*************************
Name: Garrett Genord
Class: EGR226-903
Professor Ekin
Due Date: 12/9/20
Assignment: Embedded Systems Design: Semester Project
Description: This is the .h file for the setup libary. The functions prototyped in this file are used to setup different aspects of the control system (DC motor, servo motor,
             RBG module, LCD screensaver, etc). Let it be known that the LCD and SysTick initialization functions ARE in this library!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 **************************/

#ifndef SETUP_LIB_H_
#define SETUP_LIB_H_

// The following functions that are prototyped all serve to setup/initialize different aspects of the control system
void setupDoorServo_LED(); // Prototype for the function that will set up TimerA1 PWM output for the servo (P7.6) and setup the GPIO for the pins connected to the onboard red and green LEDs (P3.7 & P3.6, respectively)
void setupDC(); // Prototype for the function that will set up TimerA1 PWM output for the DC motor (P7.4)
void setupLED_PWM(); // Prototype for the function that will set up TimerA2 PWM output for the LCD brightness and RGB LED module brightnesses (P6.7, P5.6, P5.7, & P6.6, respectively)
void LCD_adcSetup(); // Prototype for the function that will setup the ADC to read from P5.5 to change the LCD brightness
void keypad_init(); // Prototype for function that initializes the proper GPIO for keypad (P2.0-P2.6)
void screenSaverIntSetup(); // Prototype for the function that will setup Timer32 with a 10 second interrupt for displaying the screensaver
void setupAlarm(); // Prototype for the function that will set up TimerA3 PWM output for the Piezzo buzzer (P8.2)
void setupIntButtons(); // Prototype for the function that will set up GPIO and low to high triggering interrupts for the two pushbuttons (E-stop: P5.2, LED toggle: P5.0)
void LCD_Init(); // Prototype for the function that will execute the sequence to initialize the LCD (P4.1-P4.7, excluding P4.3)
void SysTick_Init(); // Prototype for the function that initializes the SysTick timer

#endif /* SETUP_LIB_H_ */
