/* 
 * File:   mcal_eeprom.h
 * Author: yusuf
 *
 * Created on April 30, 2023, 3:10 PM
 */

#ifndef MCAL_EEPROM_H
#define	MCAL_EEPROM_H
/* section : Includes   */
#include "pic18f4620.h"
#include "../mcal_std_types.h"


/* section : Micro Declaration  */

/* section : Micro Function Declaration  */

/* section : Data Type Declaration  */

/* section : Function Declaration  */
Std_ReturnType eeeprom_Read (uint16 dadd,uint8 *data);
Std_ReturnType eeeprom_Write (uint16 dadd,uint8 data);


#endif	/* MCAL_EEPROM_H */

