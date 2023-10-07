/*
 *  Module: Calculator
 *  File Name: calculator.c
 *  Description: Source file for the calculator application
 *  Author: Esraa Khaled
 */

#include "calculator.h"
#include "keypad.h"
#include "lcd.h"
#include "std_types.h"
#include <util/delay.h>


/*array to carry the values of the multi-digit numbers taken from the keypad digit by digit*/
uint32 operands[MAX_NUM_OF_OPERANDS] = {0};
/*array to carry the operators*/
uint8 operators[MAX_NUM_OF_OPERATORS];

/*counters for each array*/
uint8 operands_counter = 0, operators_counter = 0;
/*variable to store the result*/
float32 result = 0;
/*flag for knowing if the result is corrupted*/
uint8 result_flag = LOGIC_LOW;


/*Description:
 * - get values from the keypad and display it on the LCD
 * - form the multi-digit numbers and store them in the operands array
 * - store the operators in the operators array
 * - calls the calculate function on pressing =*/
void calc_init(void)
{
	uint8 i, pressedKey;
	/*pressing '=' only will break the infinite loop*/
	while (1)
	{
		/*get the pressed key value from the keypad*/
		pressedKey = KEYPAD_getPressedKey();

		/*call the calculate function on finishing entering the operation*/
		if (pressedKey == '=')
		{
			calculate();
			break; // Exit the loop when '=' is pressed
		}
		else if (pressedKey == 'c')
		{
			/*clear the display screen*/
			LCD_clearScreen();

			/*erase all values in both arrays and wait for new input*/
			for(i = 0; i < operands_counter + 1; i++)
			{
				operands[i] = 0;
			}
			for(i = 0; i < operators_counter + 1; i++)
			{
				operators[i] = 0;
			}
			operands_counter = 0;
			operators_counter = 0;
		}
		else
		{
			if ((pressedKey >= 0) && (pressedKey <= 9))
			{
				/*if key is a number display it and apply the formula for concatenating the digits*/
				LCD_integerToString(pressedKey);
				operands[operands_counter] = operands[operands_counter] * 10 + (pressedKey);
			}
			else if( (pressedKey == 'x') || (pressedKey == '%') || (pressedKey == '+') || (pressedKey == '-'))
			{
				/*if key is an operator display it and save it in the array */
				LCD_displayCharacter(pressedKey);
				operators[operators_counter] = pressedKey;
				operators_counter++; /*increment operators counter for next operator*/
				operands_counter++; /*The digits of the number will be completed on having an operator so increment operand counter to store next operand value*/
			}
		}
		_delay_ms(300); /*small delay for debounce*/
	}
}

/*calculate the operands according to the precedence of the operators*/
void calculate(void)
{
	uint8 i,j;
	/*loop on operators array to apply 'x' & '%' first */
	for(i = 0 ; i < operators_counter ; i++)
	{
		if(operators[i] == 'x')
		{
			/*apply multiplication in the same place of the first operand*/
			operands[i] *= operands[i+1];

			/*operators counter < operands counter by 1 so it is the required value for <<*/
			for(j = i; j < operators_counter; j++)
			{
				/*shift all operators after current used operator to left*/
				operators[j] = operators[j+1];
				/*shift all operands after this stored value to the left*/
				operands[j+1] = operands[j+2];
			}

			/*decrement the operators counter since we used one and overwrote on it by shifting left*/
			operators_counter--;
		}
		else if(operators[i] == '%')
		{
			if(operands[i+1] == 0)
			{
				/*check for zero division*/
				result_flag = LOGIC_LOW;
				displayResult(result);
				return;
			}
			else
			{
				/*apply division in the same place of the first operand*/
				operands[i] /= operands[i+1];
			}

			for(j = i; j < operators_counter; j++)
			{
				/*shift all operators after current used operator to left*/
				operators[j] = operators[j+1];
				/*shift all operands after this stored value to the left*/
				operands[j+1] = operands[j+2];
			}

			/*decrement the operators counter since we used one and overwrote on it by shifting left*/
			operators_counter--;
		}
	}
	/*assign the result to the first value in the operands array to be able to add and subtract from it the remaining values*/
	result = operands[0];

	/* completing the calculations of the result */
	for(i = 0 ; i < operators_counter; i++)
	{
		/*loop on the remaining operators and add or subtract values to the result*/
		if(operators[i] == '+')
			result += operands[i+1];

		else
			result -= operands[i+1];
	}
	result_flag = LOGIC_HIGH;
	displayResult(result); /*call the function to display the result*/
}


void displayResult(int result)
{
	/*strings to be printed on the LCD*/
	const sint8 string1[] = "Result = ";
	const sint8 string2[] = "ERROR";

	/*check for error flag*/
	if(result_flag == LOGIC_HIGH)
	{
		/*display the calculated result*/
		LCD_displayStringRowColumn(1,0,string1);
		LCD_integerToString(result);
	}
	else
	{
		/*display error message*/
		LCD_displayStringRowColumn(1,5,string2);
	}
}

int main(void)
{
	/*display hello message*/
	sint8 string[] = "Calculator";
	LCD_init();

	LCD_displayStringRowColumn(0,3,string);
	_delay_ms(1000);
	LCD_clearScreen();

	while(1)
	{
		calc_init();
	}
}

