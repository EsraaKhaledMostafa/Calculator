/*
 *  Module: Keypad
 *  File Name: keypad.c
 *  Description: Source file for the Keypad driver
 *  Author: Esraa Khaled
 */

#include "keypad.h"
#include "gpio.h"
#include <util/delay.h>


/*Private Functions Prototypes*/

/*check if standard keypad we will not need to adjust the key*/
#ifndef STANDARD_KEYPAD

/*check for the number of rows and call the specific function to adjust the key*/
#if (KEYPAD_COLS_NUM == 3)
static uint8 KEYPAD_4x3_adjustKeyNumber(uint8 button_number);

#elif (KEYPAD_COLS_NUM  == 4)
static uint8 KEYPAD_4x4_adjustKeyNumber(uint8 button_number);
#endif

#endif



/*Functions Definitions*/

uint8 KEYPAD_getPressedKey(void)
{
	uint8 col, row;

	/*Setting the Keypad row buttons pins as input*/
	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID,KEYPAD_FIRST_ROW_PIN_ID,PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID,KEYPAD_FIRST_ROW_PIN_ID+1,PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID,KEYPAD_FIRST_ROW_PIN_ID+2,PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID,KEYPAD_FIRST_ROW_PIN_ID+3,PIN_INPUT);

	/*Setting the Keypad column buttons pins as input*/
	GPIO_setupPinDirection(KEYPAD_COL_PORT_ID,KEYPAD_FIRST_COL_PIN_ID,PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_COL_PORT_ID,KEYPAD_FIRST_COL_PIN_ID+1,PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_COL_PORT_ID,KEYPAD_FIRST_COL_PIN_ID+2,PIN_INPUT);

	/*will not need to configure the pins of the last column if keypad is 4x3*/
#if(KEYPAD_COLS_NUM == 4)
	GPIO_setupPinDirection(KEYPAD_COL_PORT_ID,KEYPAD_FIRST_COL_PIN_ID+3,PIN_INPUT);
#endif

	while(1)
	{
		for(row = 0; row < KEYPAD_ROWS_NUM; row++)
		{
			/*On every loop all row ports should be input except one*/
			GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID+row,PIN_OUTPUT);

			GPIO_writePin(KEYPAD_ROW_PORT_ID,KEYPAD_FIRST_ROW_PIN_ID+row,KEYPAD_BUTTON_PRESSED);

			/*Loop on columns to know which column the switch is pressed on*/
			for(col = 0; col < KEYPAD_COLS_NUM; col++)
			{
				if(GPIO_readPin(KEYPAD_COL_PORT_ID,KEYPAD_FIRST_COL_PIN_ID+col) == KEYPAD_BUTTON_PRESSED)
				{
					#ifdef STANDARD_KEYPAD
						return ((row * KEYPAD_COLS_NUM)+col+1);
					#else
						#if (KEYPAD_COLS_NUM == 3)
							return (KEYPAD_4x3_adjustKeyNumber((row * KEYPAD_COLS_NUM)+col+1));
						#elif (KEYPAD_COLS_NUM == 4)
							return (KEYPAD_4x4_adjustKeyNumber((row * KEYPAD_COLS_NUM)+col+1));
						#endif
					#endif
				}
			}
			GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID,KEYPAD_FIRST_ROW_PIN_ID+row,PIN_INPUT);
			_delay_ms(5);
		}
	}
}


#ifndef STANDARD_KEYPAD
#if (KEYPAD_COLS_NUM == 3)
/*Update the keypad pressed button value with the correct 4x3 shape*/
static uint8 KEYPAD_4x3_adjustKeyNumber(uint8 button_number)
{
	uint8 keypadButton = 0;
	switch(button_number)
	{
		case 10:
			keypadButton = '*';
			break;
		case 11:
			keypadButton = '0';
			break;
		case 12:
			keypadButton = '#';
			break;
		default:
			keypadButton = button_number;
			break;
	}
	return keypadButton;
}

#elif (KEYPAD_COLS_NUM == 4)
/*Update the keypad pressed button value with the correct 4x3 shape*/
static uint8 KEYPAD_4x4_adjustKeyNumber(uint8 button_number)
{
	uint8 keypadButton = 0;
		switch(button_number)
		{
			case 1:
				keypadButton = 7;
				break;
			case 2:
				keypadButton = 8;
				break;
			case 3:
				keypadButton = 9;
				break;
			case 4:
				keypadButton = '%';
				break;
			case 5:
				keypadButton = 4;
				break;
			case 6:
				keypadButton = 5;
				break;
			case 7:
				keypadButton = 6;
				break;
			case 8:
				keypadButton = 'x';
				break;
			case 9:
				keypadButton = 1;
				break;
			case 10:
				keypadButton = 2;
				break;
			case 11:
				keypadButton = 3;
				break;
			case 12:
				keypadButton = '-';
				break;
			case 13:
				keypadButton = 'c';
				break;
			case 14:
				keypadButton = 0;
				break;
			case 15:
				keypadButton = '=';
				break;
			case 16:
				keypadButton = '+';
				break;
			default:
				keypadButton = button_number;
				break;
		}
		return keypadButton;
}
#endif

#endif




