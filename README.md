# Electronic Door Locks designed using PIC Microcontroller and simulated in Proteus
### In this project, I used the driver that I implemented for the PIC18F4620 microcontroller. This driver allowed me to control the following devices: LCD, LED, Keypad, DC motor, EEPROM, and GPIO.
- ### The user has three tries to input the password. By altering the PASS_LIMIT constant in the code, you can adjust the number of tries.
- ### The user enters the password on the keypad.
- ### The microcontroller checks the password against the password stored in EEPROM.
- ### If the password is incorrect, the red LED will turn on and the LCD will display the message "Password incorrect. You have remaining attempts left".
- ### If the password is correct, the motor activates and opens the door and The green LED turns on.
- ### The phrase "Correct Password" appears on the LCD.
- ### The user has the option to change the password or not. If the user chooses to change the password, they enter the new password on the keypad.
- ### The microcontroller stores the new password in EEPROM, which is a type of memory that retains its contents even when the power is turned off. This means that the new password will still be valid even if the electricity turns off.
- ### If the user exceeds the number of attempts, the LCD will display the message "You have exceeded the number of attempts. Please wait 60 seconds before trying again".

![Yusuf Abdulsttar](https://github.com/Yusufabdulsttar/Electronic-Door-Locks-using-PIC-Microcontroller/assets/134774623/0c5ee435-849f-401e-8b43-a338620e3b41)
