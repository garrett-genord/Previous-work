/*************************
Name: Garrett Genord
Class: EGR226-903
Professor Ekin
Due Date: 12/9/20
Assignment: Embedded Systems Design: Semester Project
Description: This program will run a control system with the following inputs: Keypad, external pushbuttons, and potentiometers. The control system will have the following outputs: LCD,
             RBG LED module, Piezzo buzzer, DC motor, Servo motor, and onboard LEDs.

             Upon downloading the program to the launchpad, the LCD will display a main menu screen where the user can choose between the door, motor, and lights menus.

             The door menu will allow the user to open, close, or arm the door. Opening the door will open the servo and illuminate the onboard green LED. Closing the door will
             close the servo and illuminate the red onboard LED. Arming the door will allow the user to enter a 3-digit password for arming/unarming the door. If the door is opened
             while it is armed, the piezzo buzzer will sound until the door is unarmed (by going back to the door menu, choosing unarm, and entering the password) or the door is closed again

             The motor menu will allow the user to enter a motor speed from 0 to 9. One of the pushbuttons serves as an E-stop and is monitored with a GPIO interrupt and if the interrupt hits,
             the motor will stop spinning

             The lights menu will allow the user to set the brightness (0-100) of the red, green, and blue LEDs on the RGB LED module. The other pushbutton serves as a toggle switch to the RGB
             LED module and will either shut the LEDs off, or return them to their previous brightness with each GPIO interrupt monitored press.

             A screensaver of a scrolling smiley face will be displayed if the keypad or pushbuttons have not been pressed in 10 seconds. This Timer32 interrupt generated.

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
#include "SysTickOperate_lib.h" // Including library for SysTick operating functions

// State assignments for FSM
#define mainMenu 0
#define doorMenu 1
#define motorMenu 2
#define lightsMenu 3
#define setRLED 4
#define setGLED 5
#define setBLED 6
#define armDoor 7
#define unarmDoor 8

int currentState = mainMenu; // Global variable to hold the current state

void resetDisplay(); // Prototype for the function that will reset the display to whatever it was when the 10 second interrupt hit
int Read_Keypad(); // Prototype for function that will read what character has been pressed on the keypad and set num equal to it
uint8_t num; // Global variable to hold the number that was pressed on the keypad

int doorArmedFlag = 0; // Global variable that denotes whether the door is armed or not (used in main and the resetDisplay function)

volatile int screenSaverFlag = 0; // Global volatile flag that is set in the Timer32 ISR whenever 10 seconds has passed without input

int decode3Digits(int firstPress); // Prototype for the function that will decode the next 3 digits pressed on the keypad (starting with the first press that is detected in main and then passed to the function) and return the value back to main as an int

volatile int LEDtoggleFlag = 0; // Global volatile flag that is set in the P5 ISR whenever the LED toggle button is pressed


void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    // Calling the functions to setup all the aspects of the control system
    setupDC();
    setupDoorServo_LED();
    SysTick_Init();
    LCD_Init();
    keypad_init();
    setupAlarm();
    setupLED_PWM();
    LCD_adcSetup();
    screenSaverIntSetup();
    setupIntButtons();
    displayMainMenu();

    __enable_irq(); // Enabling all interrupts

    int LEDenableFlag = 1; // Flag that is used to denote whether the RGB LED module is enabled or disabled
    double RGBdutyCycle = 0; // Variable to hold the duty cycle that will be passed to the setRGBLEDs function
    int doorPassword = -1; // Variable to hold the password that was used to arm the door
    int userPassword = -1; // Variable to hold the password that the user enters to try and unarm the door
    uint16_t CCRvalue1, CCRvalue2, CCRvalue3; // Variables to hold the previous CCR values of TimerA2 when toggling the RGB LEDs

    while(1){ // Infinite while-loop

        LCDbrightnessSet(); // Setting the LCD brightness based off the potentiometer each time in the loop

        if(screenSaverFlag){ // If the screensaver flag has been set after 10 seconds in the Timer32 ISR, enter the if-statement
            screenSaver(); // Calling the screensaver function to scroll the smiley face until a key is pressed
            screenSaverFlag = 0; // Reset the screensaver flag
            TIMER32_1 -> LOAD = 30000000; // Reload the Timer32 value to start the 10 second count over again
            resetDisplay(); // Calling the reset display function to set the display back to what it was before the screensaver
        }

        if(LEDtoggleFlag){ // If the LED toggle flag has been set in the P5 ISR after pressing the external toggle pushbutton, enter the if-statement
            LEDtoggleFlag = 0; // Immediately set the flag back to 0

            if(LEDenableFlag){ // If the LEDs are currently enabled, execute the code to store the current values and disable them
                CCRvalue1 = (TIMER_A2 -> CCR[1]); // Store the current brightnesses of the LEDs
                CCRvalue2 = (TIMER_A2 -> CCR[2]);
                CCRvalue3 = (TIMER_A2 -> CCR[3]);
                TIMER_A2 -> CCR[1] = 0; // Set the brightnesses to 0
                TIMER_A2 -> CCR[2] = 0;
                TIMER_A2 -> CCR[3] = 0;
                LEDenableFlag = 0; // Set the flag to 0 to signal that the LEDs are disabled
            } else { // If the LEDs are currently disabled, execute the code to restore their previous values
                TIMER_A2 -> CCR[1] = CCRvalue1; // Restoring previous brightnesses
                TIMER_A2 -> CCR[2] = CCRvalue2;
                TIMER_A2 -> CCR[3] = CCRvalue3;
                LEDenableFlag = 1; // Set the flag to 1 to signal that the LEDs are enabled
            }
        }

        // The following if-statement serves as the FSM of the control system, it is only entered when a keypad press is detected
        if(Read_Keypad()){
            TIMER32_1 -> LOAD = 30000000; // Every time a key has been pressed, reset the 10 second countdown

            switch (currentState){ // Switch statement for FSM
            case mainMenu: // Main menu state
                if(num == 1 && doorArmedFlag == 0){ // If 1 has been pressed and the door is not armed, go to the door menu and display the door unarmed menu
                    currentState = doorMenu;
                    displayDoorUnarmedMenu();
                }

                if(num == 1 && doorArmedFlag == 1){ // If 1 has been pressed and the door is armed, go to the door menu and display the door armed menu
                    currentState = doorMenu;
                    displayDoorArmedMenu();
                }

                if(num == 2){ // If 2 has been pressed, go to the motor menu and display the motor menu
                    currentState = motorMenu;
                    displayMotorMenu();
                }

                if(num == 3){ // If 3 has been pressed, go to the lights menu and display the lights menu
                    currentState = lightsMenu;
                    displayLightsMenu();
                }
                break;
            case doorMenu: // Door menu state
                if(num == 1 && doorArmedFlag == 0){ // If 1 has been pressed and the door isnt armed, open the door, keep the alarm off, then display the main menu and go back to it
                    setDoor('o');
                    setDoorAlarm(0);
                    displayMainMenu();
                    currentState = mainMenu;
                } else if (num == 1 && doorArmedFlag == 1){ // If 1 has been pressed and the door is armed, open the door, sound the alarm, then display the main menu and go back to it
                    setDoor('o');
                    setDoorAlarm(1);
                    displayMainMenu();
                    currentState = mainMenu;
                }

                if(num == 2){ // If 2 has been pressed, close the door, keep the alarm off, then display  the main menu and go back to it
                    setDoor('c');
                    displayMainMenu();
                    currentState = mainMenu;
                    setDoorAlarm(0);
                }

                if(num == 3 && doorArmedFlag == 0){ // If 3 has been pressed, and the door isnt armed, move to the door arming state
                    currentState = armDoor;

                } else if (num == 3 && doorArmedFlag == 1){ // If 3 has been pressed and the door is armed, move to the door unarming state
                    currentState = unarmDoor;
                }

                break;
            case armDoor: // Door arming state
                doorPassword = decode3Digits(num); // Decoding the 3 digit password that has been typed in with the decode3Digits function and store the integer it returns in a variable
                doorArmedFlag = 1; // Set the door armed flag
                setDoor('c'); // Close the door if it is open while being armed
                currentState = mainMenu; // Go back to main menu state
                displayMainMenu(); // Display main menu
                break;
            case unarmDoor: // Door unarming state
                userPassword = decode3Digits(num); // Decoding the 3 digit user password that has been typed in with the decode3Digits function and store the integer it returns in a variable

                if(userPassword == doorPassword){ // If the password matches, execute the following if-statement
                    setDoorAlarm(0); // Keep door alarm off
                    doorArmedFlag = 0; // Reset door armed flag
                    displayValidPIN(); // Inform the user that their PIN was valid by displaying a message on the LCD
                    delay_ms(3000); // Wait 3 seconds before changing the display
                    TIMER32_1 -> LOAD = 30000000; // Reset the 10 second count after forcing the user to wait 3 seconds
                    currentState = mainMenu; // Go back to main menu state
                    displayMainMenu(); // Display main menu
                } else { // If the password doesnt match, execute the following if-statement
                    doorArmedFlag = 1; // Make sure the door flag still says that its armed
                    displayInvalidPIN(); // Inform the user that their PIN was invalid by displaying a message on the LCD
                    delay_ms(3000); // Wait 3 seconds before changing the display
                    TIMER32_1 -> LOAD = 30000000; // Reset the 10 second count after forcing the user to wait 3 seconds
                    currentState = mainMenu; // Go back to main menu state
                    displayMainMenu(); // Display main menu
                }
                break;
            case motorMenu: // Motor menu state
                if(num < 10){ // Error checking to not allow user to press * or #
                    setDC((num/10.0)); // Set the duty cycle of the DC motor
                    currentState = mainMenu; // Go back to main menu state
                    displayMainMenu(); // Display main menu
                }
                break;
            case lightsMenu: // Lights menu state
                if(num == 1){ // If 1 has been pressed, display the red light menu and go to the set red LED state
                    displayLightsMenu_2('R');
                    currentState = setRLED;
                }
                if(num == 2){ // If 2 has been pressed, display the green light menu and go to the set green LED state
                    displayLightsMenu_2('G');
                    currentState = setGLED;
                }
                if(num == 3){ // If 3 has beeen pressed, display the blue light menu and go to the set blue LED state
                    displayLightsMenu_2('B');
                    currentState = setBLED;
                }
                break;
            case setRLED: // Setting red LED state
                RGBdutyCycle = ((decode3Digits(num))/100.0); // Calculating the duty cycle based off the 3 numbers that the user enters
                while(RGBdutyCycle > 1){ // Error checking to make sure user can't enter a number greater than 100
                    delay_micro(38); // Clearing the previous entry that was printed on the LCD
                    commandWrite(0x97);
                    delay_micro(38);
                    printStringToLCD("     ");
                    delay_micro(38);
                    commandWrite(0x97);
                    delay_micro(38);

                    if(Read_Keypad()){ // Only begin to decode the digits when the user enters the first one
                        TIMER32_1 -> LOAD = 30000000; // Reset the 10 second count after keypad input
                        RGBdutyCycle = ((decode3Digits(num))/100.0);
                    }
                }
                setRGBLEDs('R', RGBdutyCycle); // Setting the red LED duty cycle
                currentState = mainMenu; // Go back to main menu state
                displayMainMenu(); // Display main menu
                break;
            case setGLED: // Setting green LED state
                RGBdutyCycle = ((decode3Digits(num))/100.0); // Calculating the duty cycle based off the 3 numbers that the user enters
                while(RGBdutyCycle > 1){ // Error checking to make sure user can't enter a number greater than 100
                    delay_micro(38); // Clearing the previous entry that was printed on the LCD
                    commandWrite(0x97);
                    delay_micro(38);
                    printStringToLCD("     ");
                    delay_micro(38);
                    commandWrite(0x97);
                    delay_micro(38);

                    if(Read_Keypad()){ // Only begin to decode the digits when the user enters the first one
                        TIMER32_1 -> LOAD = 30000000; // Reset the 10 second count after keypad input
                        RGBdutyCycle = ((decode3Digits(num))/100.0);
                    }
                }
                setRGBLEDs('G', RGBdutyCycle); // Setting the green LED duty cycle
                currentState = mainMenu; // Go back to main menu state
                displayMainMenu(); // Display main menu
                break;
            case setBLED: // Setting blue LED state
                RGBdutyCycle = ((decode3Digits(num))/100.0); // Calculating the duty cycle based off the 3 numbers that the user enters
                while(RGBdutyCycle > 1){ // Error checking to make sure user can't enter a number greater than 100
                    delay_micro(38); // Clearing the previous entry that was printed on the LCD
                    commandWrite(0x97);
                    delay_micro(38);
                    printStringToLCD("     ");
                    delay_micro(38);
                    commandWrite(0x97);
                    delay_micro(38);

                    if(Read_Keypad()){ // Only begin to decode the digits when the user enters the first one
                        TIMER32_1 -> LOAD = 30000000; // Reset the 10 second count after keypad input
                        RGBdutyCycle = ((decode3Digits(num))/100.0);
                    }
                }
                setRGBLEDs('B', RGBdutyCycle); // Setting the blue LED duty cycle
                currentState = mainMenu; // Go back to main menu state
                displayMainMenu(); // Display main menu
                break;
            default: // Default state

                break;
            }
        }

    }
} // End of main


void resetDisplay(){ // This function will reset the current display after the screensaver is finished

    // This switch statement will display the appropriate menu based off the current state global variable
    switch (currentState){
    case mainMenu:
        displayMainMenu();
        break;
    case doorMenu:
        if(doorArmedFlag) displayDoorArmedMenu();
        else displayDoorUnarmedMenu();
        break;
    case motorMenu:
        displayMotorMenu();
        break;
    case lightsMenu:
        displayLightsMenu();
        break;
    case setRLED:
        displayLightsMenu_2('R');
        break;
    case setGLED:
        displayLightsMenu_2('G');
        break;
    case setBLED:
        displayLightsMenu_2('B');
        break;

    }
}

int Read_Keypad(){ // Function that will read what character has been pressed on the keypad and set num equal to it, returns 1 if a key is pressed, 0 otherwise

    uint8_t col, row;
    for(col = 0; col < 3; col++){
        P2DIR = 0; // First set all columns to inputs
        P2DIR |= BIT(4 + col); // Set the current column to output
        P2OUT &= ~ BIT(4 + col); // Drive the current column low (similar to making it a GND )

        delay_ms(10); // Delaying so propagation delay will not be an issue
        row = P2IN & 0x0F; // Using a mask to check if any of the bits for the rows are 0, which means that row has had a button pressed

        while ( !(P2IN & BIT0) | !(P2IN & BIT1) | !( P2IN & BIT2) | !( P2IN & BIT3) ); // Waiting for the keys to be released before moving on

        if(row != 0x0F) // If any of the inputs is low, one of the keys must have been pressed
            break;
    }

    P2DIR = 0; // Setting all pins back to inputs
    if(col == 3) // If we have successfully made it to the end of the for loop, no key has been pressed
        return 0;

    if(row == 0x0E) num = col + 1; // Key in row 0 has been pressed
    if(row == 0x0D) num = 3 + col + 1; // Key in row 1 has been pressed
    if(row == 0x0B) num = 6 + col + 1; // Key in row 2 has been pressed
    if(row == 0x07) num = 9 + col + 1; // Key in row 3 has been pressed

    if(num == 11) num = 0; // Decoding 0

    return 1; // Return 1 if made it this far, a key must have been pressed
}

int decode3Digits(int firstPress){ // This function will decode 3 digits that the user has entered (starting with the firstPass integer passed to it) and then return the decoded 3 digit password as an int to main

    int numHolder [4] = 0; // Array to hold each individual digit
    int counter = 1; // Counter to index through the array above
    int finalNum = 0; // Variable to hold the final decoded number

    numHolder[0] = firstPress; // Storing the first press into the array
    dataWrite(' '); // Printing a space to the LCD before printing the password

    dataWrite((numHolder[0] + '0')); // Printing the number to the LCD by taking the integer and adding '0' to convert it to ASCII
    while(1){
        if(Read_Keypad()){ // If the keypad is read, execute the following

            TIMER32_1 -> LOAD = 30000000; // Reset the 10 second interrupt

            if(counter == 1){ // Storing the next press in the array and printing it
                numHolder[1] = num;
                counter++; // Increment the counter
                dataWrite((numHolder[1] + '0'));
            } else if (counter == 2){ // Storing the next
                numHolder[2] = num;
                dataWrite((numHolder[2] + '0'));
                break;
            }
        }
    }

    finalNum = ((100 * numHolder[0]) + (10 * numHolder[1]) + numHolder[2]); // Returning the decoded final num
    delay_ms(500);

    return (finalNum);
}


/////////////////// Start of ISRs
void PORT5_IRQHandler(void){ // P5 interrupt handler, this function will debounce whatever button has been pressed and set then LED toggle flag or shut off the DC motor
    uint16_t status = 1; // Declaring variable to hold the status of the button debounce



    if(P5IFG & BIT0){ // If the flag for the decrement button is triggered, debounce that button
        P5IFG = 0;
        while(1){
            status = ((status << 1) | ((P5IN & BIT0) >> 0)); // Shifting the button input onto the end of the status variable

            if(status == 0){ // If the button is never high for more than 32 cycles, it has not been pressed, exit the loop
                break;
            } else if(status == 0xFF){ // If the button has been high for 32 cycles or more, decrement the duty cycle (as long as it isnt already at 0%), change the PWM output accordingly, and exit the loop
                LEDtoggleFlag = 1;
                TIMER32_1 -> LOAD = 30000000;
                break;
            }
            __delay_cycles(250); // Delaying to avoid switch bounce issues
        }
    }

    if(P5IFG & BIT2){ // If the flag for the stop button is triggered, debounce that button
        P5IFG = 0;
        while(1){
            status = ((status << 1) | ((P5IN & BIT2) >> 2));

            if(status == 0){ // If the button is never high for more than 32 cycles, it has not been pressed, exit the loop
                break;
            } else if(status == 0xFF){ // If the button has been high for 32 cycles or more, button has been pushed, so set the duty cycle to 0
                TIMER_A1 -> CCR[4] = 0;
                TIMER32_1 -> LOAD = 30000000;
                break;
            }
            __delay_cycles(250); // Delaying to avoid switch bounce issues
        }
    }

}

void T32_INT1_IRQHandler(void){ // Timer32 ISR that is triggered if there is no input for 10 seconds, it will set a flag and clear the interrupt register
    TIMER32_1 -> INTCLR = 1;
    screenSaverFlag = 1;
}
/////////////////// End of ISRs
