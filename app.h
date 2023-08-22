/* 
 * File:   app.h
 * Author: yusuf
 *
 * Created on April 16, 2023, 2:54 PM
 */

#ifndef APP_H
#define	APP_H

/* section : Includes   */
#include "ECU_Layer/LED/ecu_led.h"
#include "ECU_Layer/DC_MOTOR/ecu_motor.h"
#include "ECU_Layer/Key_Bad/key_bad.h"
#include "ECU_Layer/LCD/Ecu_LCD.h"

/* section : Micro Declaration  */
#define PASS_LIMIT       3
/* section : Micro Function Declaration  */

/* section : Data Type Declaration  */

led_t led_Green;
led_t led_Red;
dc_motor_t DC_Motor;
keypad_t keypad;
lcd_4bit_t lcd;


uint32 Password = 111111;
uint32 Entered_Password = 0;
uint8 keypad_value;
uint8 Numb_Of_Pass = 0;
uint8 lcd_output_data_col = 0;
uint8 count_char = 0;
uint8 Password_incorrect = 0;

/* section : Function Declaration  */
void initialization ();
uint8 Keypad_Read_Value();

#endif	/* APP_H */
