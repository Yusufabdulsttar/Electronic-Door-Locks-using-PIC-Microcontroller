/* 
 * File:   mcal_eeprom.c
 * Author: yusuf
 *
 * Created on April 30, 2023, 3:10 PM
 */
#include "mcal_eeprom.h"


Std_ReturnType eeeprom_Read (uint16 dadd,uint8 *data){
    Std_ReturnType ret = E_OK;
    if (NULL == data){
        ret = E_NOT_OK;
    }
    else {
        /* Update the Address Registers */
        EEADRH = (uint8)((dadd >> 8)& 0x03);
        EEADR = (uint8)(dadd & 0xff);
        /* Select Access data EEPROM memory */
        EECON1bits.EEPGD = 0;
        EECON1bits.CFGS = 0;
        /* Initiates a data EEPROM read cycle */
        EECON1bits.RD = 1;
        *data = EEDATA;
    }
    return ret; 
    
}


Std_ReturnType eeeprom_Write (uint16 dadd,uint8 data){
    Std_ReturnType ret = E_OK;
    /* Read the Interrupt Status "Enabled or Disabled" */
    uint8 Global_interrupt_state = INTCONbits.GIE;
    /* Update the Address Registers */
    EEADRH = (uint8)((dadd >> 8)& 0x03);
    EEADR = (uint8)(dadd & 0xff);
    /* Update the Data Register */
    EEDATA = data;
    /* Select Access data EEPROM memory */
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    /* Allows write cycles to Flash program/data EEPROM */
    EECON1bits.WREN = 1;
    /* Disable all interrupts "General Interrupt" */
    INTCONbits.GIE = 0;
    /* Write the required sequence : 0x55 -> 0xAA */
    EECON2 = 0x55;
    EECON2 = 0xAA;
    /* Initiates a data EEPROM erase/write cycle */
    EECON1bits.WR = 1;
    /* Wait for write to complete */
    while(EECON1bits.WR);
    /* Inhibits write cycles to Flash program/data EEPROM */
    EECON1bits.WREN = 0;
    /* Restore the Interrupt Status "Enabled or Disabled" */
    INTCONbits.GIE = Global_interrupt_state;
    return ret; 
}
