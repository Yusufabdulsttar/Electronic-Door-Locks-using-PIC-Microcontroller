/*
 * File:   app.c
 * Author: yusuf
 *
 * Created on April 16, 2023, 2:12 PM
 */
#include "app.h"


int main() {
    /* For initialization Drivers like (LED - KEYPAD - LCD - DC_Motor) */
    initialization (); 
    /* check if we set a new password or not*/
    eeeprom_Read (0x04,&valid_pass);
    /* if we didn't set a new password or use factory reset we use the default password*/
    if(valid_pass != 0x01){
    eeeprom_Write (0x00,(uint8)Defualt_Password);
    eeeprom_Write (0x01,(uint8)(Defualt_Password>>8));
    eeeprom_Write (0x02,(uint8)(Defualt_Password>>16));
    }
    
    lcd_4bit_send_string_pos(&lcd, 2, 1, "Enter Your Password:");
    
    while(1)
    {
        /* Read Keypad value */
        keypad_value = Keypad_Read_Value();
        /* Check for valid password */
        /* if key = 1 thats mean we need to set a new password*/
        if( (Key == 1) ){
            lcd_4bit_send_string_pos(&lcd, 2, 1, "Enter New Password:");
        }
        if(('0' <= keypad_value) && ('9' >= keypad_value) && (Key == 1) )
        {
            Numb_Of_Pass2++;
             /* Store the password */
            New_Password = (New_Password * 10) + (keypad_value - '0');
            lcd_4bit_send_char_data_pos(&lcd, 3, ++lcd_output_data_col, keypad_value);
            __delay_ms(200);
            lcd_4bit_send_char_data_pos(&lcd, 3, lcd_output_data_col, '*');
        }
        if(6 == Numb_Of_Pass2)
        {
            /* Store New Password in EEPROM*/
            eeeprom_Write (0x00,(uint8)New_Password);
            eeeprom_Write (0x01,(uint8)(New_Password>>8));
            eeeprom_Write (0x02,(uint8)(New_Password>>16));
            New_Password = 0;
            /* Tell The Program We Set A New Password */
            eeeprom_Write (0x04,0x01);
            Numb_Of_Pass2 = 0;
            lcd_output_data_col = 0;
            lcd_4bit_send_string_pos(&lcd, 2, 1, "New Password is set");
            __delay_ms(200);
            lcd_4bit_send_command(&lcd, LCD_CLEAR);
            __delay_ms(10);
            lcd_4bit_send_string_pos(&lcd, 2, 1, "Enter Your Password:");
            Key = 0;
            keypad_value = Keypad_Read_Value();
        }
        else{ /* Nothing */ }
        /* if key = 0 thats mean we don't need to set a new password*/
        if(('0' <= keypad_value) && ('9' >= keypad_value)&& (Key == 0))
        {
           Numb_Of_Pass++;
            /* Store the password */
           Entered_Password = (Entered_Password * 10) + (keypad_value - '0');
           lcd_4bit_send_char_data_pos(&lcd, 3, ++lcd_output_data_col, keypad_value);
           __delay_ms(200);
           lcd_4bit_send_char_data_pos(&lcd, 3, lcd_output_data_col, '*');
        }
        /* check if the user entered (6) digit number (Maximum number of password) */
        if(6 == Numb_Of_Pass)
        {
            /* Read The Password From EEPROM  */
            Stored_Password = 0;
            eeeprom_Read (0x00,&temp0);
            eeeprom_Read (0x01,&temp1);
            eeeprom_Read (0x02,&temp2);
            Stored_Password = (uint8)temp0 | (uint32)temp1<<8 | (uint32)temp2<<16;
            Numb_Of_Pass = 0;
            lcd_output_data_col = 0;
            
            /* Turn on the Green led and open the door (Moving DC Motor) if the password is valid */
            if(Entered_Password == Stored_Password)
            {
                led_on(&led_Green);                // Turn on the Green led
                dc_motor_forward(&DC_Motor);       // open the door (Moving DC Motor)
                lcd_4bit_send_command(&lcd, LCD_CLEAR);
                lcd_4bit_send_string_pos(&lcd , 1, 7, "Correct Password"); 
                lcd_4bit_send_string_pos(&lcd , 3, 3, "****Welcome!****"); 
                Password_incorrect = 0;
                __delay_ms(2000);
                led_off(&led_Green);
                dc_motor_off(&DC_Motor);
                lcd_4bit_send_command(&lcd, LCD_CLEAR);
                /* The user can choose to change their password or not */
                lcd_4bit_send_string_pos(&lcd , 1, 1, "(1) Change The Pass"); 
                lcd_4bit_send_string_pos(&lcd , 3, 1, "Any Key To Exit"); 
                /* wait for the user to decide */
                while(keypad_newvalue == 0){
                    keypad_newvalue = Keypad_Read_Value();
                    if (keypad_newvalue == '1'){Key = 1;}
                    else{ Key = 0;}
                }
                lcd_4bit_send_command(&lcd, LCD_CLEAR);
                __delay_ms(250);
                lcd_4bit_send_string_pos(&lcd, 2, 1, "Enter Your Password:");
                keypad_newvalue = 0;
            }
            /* If the password is invalid, Turn on the Red led */
            else
            {
                led_on(&led_Red);                // Turn on the Red led
                lcd_4bit_send_command(&lcd, LCD_CLEAR);
                lcd_4bit_send_string_pos(&lcd, 2, 1, "Incorrect Password!");
                Password_incorrect++;      
                lcd_4bit_send_string_pos(&lcd, 3, 3, "More Tries Left");
                count_char = 0;
                count_char = '0' + ((PASS_LIMIT - Password_incorrect) % 10);    
                lcd_4bit_send_char_data_pos(&lcd, 3, 1,count_char);
                __delay_ms(2000);
                led_off(&led_Red);
                lcd_4bit_send_command(&lcd, LCD_CLEAR);
                __delay_ms(10);
                lcd_4bit_send_string_pos(&lcd, 2, 1, "Enter Your Password:");
            }
            /* Wait 60 seconds if the password is typed three times incorrectly */
            if(PASS_LIMIT == Password_incorrect){
                lcd_4bit_send_command(&lcd, LCD_CLEAR);
                __delay_ms(10);
                lcd_4bit_send_string_pos(&lcd, 2, 1, "Please Wait    Sec");
                count_char = 0;
                for(uint8 counter = 60 ; counter > 0 ; counter--){
                    count_char = '0' + (counter % 10);    
                    lcd_4bit_send_char_data_pos(&lcd, 2, 14, count_char);
                    count_char = '0' + ((counter/10) % 10);
                    lcd_4bit_send_char_data_pos(&lcd, 2, 13, count_char);
                    led_on(&led_Red);
                    __delay_ms(250);
                    led_off(&led_Red);
                    __delay_ms(250);
                }
                Password_incorrect = 0;
                lcd_4bit_send_command(&lcd, LCD_CLEAR);
            }
            Entered_Password = 0;
        }
        else{ /* Nothing */ }        
    }
    return 0;
}

uint8 Keypad_Read_Value()
{
    uint8 Value;
    keypad_read(&keypad, &Value);
    return Value;  
}

void initialization (){

led_Green.port_name = PORTE_INDEX, led_Green.pin = PIN0, led_Green.led_status = LOW;
led_Red.port_name = PORTE_INDEX, led_Red.pin = PIN1, led_Red.led_status = LOW;

    DC_Motor.dc_motor_pin[0].port = PORTA_INDEX,
    DC_Motor.dc_motor_pin[0].pin = PIN0,
    DC_Motor.dc_motor_pin[0].direction = dirc_OUTPUT,
    DC_Motor.dc_motor_pin[0].logic = LOW,
    DC_Motor.dc_motor_pin[1].port = PORTA_INDEX,
    DC_Motor.dc_motor_pin[1].pin = PIN1,
    DC_Motor.dc_motor_pin[1].direction = dirc_OUTPUT,
    DC_Motor.dc_motor_pin[1].logic = LOW;


    keypad.keypad_row[0].port = PORTC_INDEX,
    keypad.keypad_row[0].pin = PIN0,
    keypad.keypad_row[0].direction = dirc_OUTPUT,
    keypad.keypad_row[0].logic = LOW,
    keypad.keypad_row[1].port = PORTC_INDEX,
    keypad.keypad_row[1].pin = PIN1,
    keypad.keypad_row[1].direction = dirc_OUTPUT,
    keypad.keypad_row[1].logic = LOW,
    keypad.keypad_row[2].port = PORTC_INDEX,
    keypad.keypad_row[2].pin = PIN2,
    keypad.keypad_row[2].direction = dirc_OUTPUT,
    keypad.keypad_row[2].logic = LOW,
    keypad.keypad_row[3].port = PORTC_INDEX,
    keypad.keypad_row[3].pin = PIN3,
    keypad.keypad_row[3].direction = dirc_OUTPUT,
    keypad.keypad_row[3].logic = LOW,
    
    keypad.keypad_col[0].port = PORTC_INDEX,
    keypad.keypad_col[0].pin = PIN4,
    keypad.keypad_col[0].direction = dirc_INPUT,
    keypad.keypad_col[0].logic = LOW,
    keypad.keypad_col[1].port = PORTC_INDEX,
    keypad.keypad_col[1].pin = PIN5,
    keypad.keypad_col[1].direction = dirc_INPUT,
    keypad.keypad_col[1].logic = LOW,
    keypad.keypad_col[2].port = PORTC_INDEX,
    keypad.keypad_col[2].pin = PIN6,
    keypad.keypad_col[2].direction = dirc_INPUT,
    keypad.keypad_col[2].logic = LOW;
    keypad.keypad_col[3].port = PORTC_INDEX,
    keypad.keypad_col[3].pin = PIN7,
    keypad.keypad_col[3].direction = dirc_INPUT,
    keypad.keypad_col[3].logic = LOW;


    lcd.lcd_rs.port = PORTD_INDEX,
    lcd.lcd_rs.pin = PIN0,
    lcd.lcd_rs.direction = dirc_OUTPUT,
    lcd.lcd_rs.logic = LOW,
    lcd.lcd_en.port = PORTD_INDEX,
    lcd.lcd_en.pin = PIN1,
    lcd.lcd_en.direction = dirc_OUTPUT,
    lcd.lcd_en.logic = LOW,
    
    lcd.lcd_data[0].port = PORTD_INDEX,
    lcd.lcd_data[0].pin = PIN2,
    lcd.lcd_data[0].direction = dirc_OUTPUT,
    lcd.lcd_data[0].logic = LOW,
    lcd.lcd_data[1].port = PORTD_INDEX,
    lcd.lcd_data[1].pin = PIN3,
    lcd.lcd_data[1].direction = dirc_OUTPUT,
    lcd.lcd_data[1].logic = LOW,
    lcd.lcd_data[2].port = PORTD_INDEX,
    lcd.lcd_data[2].pin = PIN4,
    lcd.lcd_data[2].direction = dirc_OUTPUT,
    lcd.lcd_data[2].logic = LOW,
    lcd.lcd_data[3].port = PORTD_INDEX,
    lcd.lcd_data[3].pin = PIN5,
    lcd.lcd_data[3].direction = dirc_OUTPUT,
    lcd.lcd_data[3].logic = LOW;

    led_initialize(&led_Green);
    led_initialize(&led_Red);
 
    dc_motor_initialize(&DC_Motor);
    
    keypad_initialize(&keypad);
    
    lcd_4bit_intialize(&lcd);
}   
 
