/*
 *  Module: Keypad
 *  File Name: keypad.h
 *  Description: Header file for the Keypad driver
 *  Author: Esraa Khaled
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "std_types.h"


/*Definitions*/

/*#define STANDARD_KEYPAD*/
#define KEYPAD_COLS_NUM 4
#define KEYPAD_ROWS_NUM 4

/*Keypad Port Configuration*/
/*The column of the keypad connected to which pins on which port*/
#define KEYPAD_COL_PORT_ID         PORTD_ID
#define KEYPAD_FIRST_COL_PIN_ID    PIN2_ID

/*The row of the keypad connected to which pins on which port*/
#define KEYPAD_ROW_PORT_ID         PORTB_ID
#define KEYPAD_FIRST_ROW_PIN_ID    PIN4_ID

/*Keypad Button Logic Configuration*/
#define KEYPAD_BUTTON_PRESSED      LOGIC_LOW
#define KEYPAD_BUTTON_RELEASED     LOGIC_HIGH


/*Public Functions Prototypes*/
uint8 KEYPAD_getPressedKey(void);

#endif /* KEYPAD_H_ */
