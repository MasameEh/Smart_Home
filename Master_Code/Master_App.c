/* 
 * File:   Master_App.c
 * Author: Mohamed Sameh
 *
 * Created on December 27, 2023, 11:21 PM
 */
#include "Master_App.h"

uint8 keypad_value = NO_KEY_PRESSED;
uint8 login_mode = NO_MODE;
uint8 Admin_Pass_Status = PASS_NOT_SET;
uint8 Guest_Pass_Status = PASS_NOT_SET;

volatile uint16 session_counter = 0;//indicate session time
uint8 block_mode_flag = FALSE;//is true if the login is blocked or false if is not blocked
uint8 timeout_flag = FALSE;//stores if the session is still valid or outdated

uint8 temperature = 0;//The average temperature of the room
uint8 temp_ones = NOT_SELECTED;//The entered right number of the temperature
uint8 temp_tens = NOT_SELECTED;//The entered left number of the temperature

int main() 
{
    /*****************  INITIALIZE  ***********************/
    application_init();
    /******************************************************/
    lcd_8bit_send_string(&LCD,"Welcome to Smart");
    lcd_8bit_send_string_pos(&LCD, "Home System", 2,1);
    
    __delay_ms(1000);//Halt the system for the given time in (ms)
    //remove all previously printed characters on the LCD
    lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
    /***************************/
	/* Setting Admin and Guest passwords if not set */
	//read the state of the the passwords of the admin and guest if both are set or not set
    EEPROM_ReadByte(ADMIN_PASS_STATUS_ADDRESS, &Admin_Pass_Status);
    EEPROM_ReadByte(GUEST_PASS_STATUS_ADDRESS, &Guest_Pass_Status);
    if((PASS_SET != Admin_Pass_Status) || (PASS_SET != Guest_Pass_Status))
    {
        lcd_8bit_send_string(&LCD,"Login for");
        lcd_8bit_send_string_pos(&LCD, "first time", 2,1);
        __delay_ms(1000);//Halt the system for the given time in (ms)
        lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
        
        lcd_8bit_send_string(&LCD," Set Admin Pass");
        lcd_8bit_send_string_pos(&LCD, "Admin pass:", 2,1);
        
        /********************************* setting Admin password **********************************************/
        /* the counter of the characters of the password */
        uint8 password_counter = 0;
        uint8 password[PASS_SIZE] = {NOT_STORED,NOT_STORED,NOT_STORED,NOT_STORED};
        /* loop until the user finishes inserting the password */
        while (password_counter < PASS_SIZE)
		{
            keypad_value = NO_KEY_PRESSED;
            while(keypad_value == NO_KEY_PRESSED)
            {
                keypad_value = keypad_get_value(&keypad);
            }
            password[password_counter] = keypad_value; //add the pressed character to the pass array
            lcd_8bit_send_char(&LCD, keypad_value);
            __delay_ms(CHARACTER_PREVIEW_TIME);//Halt the system for the given time in (ms)
            lcd_8bit_send_char_pos(&LCD, PASSWORD_SYMBOL, 2, 12+password_counter);
            __delay_ms(100);//Halt the system for the given time in (ms)
            password_counter++;//increase the characters count
        }
        EEPROM_WriteBlock(EEPROM_ADMIN_ADDRESS, password, PASS_SIZE);//save the entire password as a block to the EEPROM
        EEPROM_WriteByte(ADMIN_PASS_STATUS_ADDRESS, PASS_SET);       //write the status of pass as it is set
        
        lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
        lcd_8bit_send_string(&LCD,"Pass Saved");
        __delay_ms(500);
        lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
        
        /********************************* setting guest password **********************************************/
        password_counter = 0;
        lcd_8bit_send_string(&LCD,"Set Guest Pass");
        lcd_8bit_send_string_pos(&LCD, "Guest pass:", 2,1);
        /* loop until the user finishes inserting the password */
        while (password_counter < PASS_SIZE)
		{
            keypad_value = NO_KEY_PRESSED;
            while(keypad_value == NO_KEY_PRESSED)
            {
                keypad_value = keypad_get_value(&keypad);
            }
            password[password_counter] = keypad_value;//add the pressed character to the pass array
            lcd_8bit_send_char(&LCD, keypad_value);
            __delay_ms(CHARACTER_PREVIEW_TIME);//Halt the system for the given time in (ms)
            lcd_8bit_send_char_pos(&LCD, PASSWORD_SYMBOL, 2, 12+password_counter);
            __delay_ms(100);//Halt the system for the given time in (ms)
            password_counter++;//increase the characters count
        }
        EEPROM_WriteBlock(EEPROM_GUEST_ADDRESS, password, PASS_SIZE);//save the entire password as a block to the EEPROM
        EEPROM_WriteByte(GUEST_PASS_STATUS_ADDRESS, PASS_SET);//write the status of pass as it is set
        
        lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
        lcd_8bit_send_string(&LCD,"Pass Saved");
        __delay_ms(500);
        lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
    }//The end of whether admin and guest passwords are set or not
    /* this code runs only if the system is not running for the first time (ADMIN and GUEST passwords are set)*/
    else
    {
        EEPROM_ReadByte(LOGIN_BLOCKED_ADDRESS, &block_mode_flag);
    }
    while(1)
    {
        keypad_value = NO_KEY_PRESSED;
        uint8 pass_tries_count = 0; //stores how many times the user tried to log in and failed
        
        if(timeout_flag == TRUE)
        {
            Timer0_DeInit(&timer);
            session_counter = 0;//clear session counter
            timeout_flag=FALSE;//clear time out flag
			login_mode=NO_MODE;//log the user out
            keypad_value = NO_KEY_PRESSED;
            led_turn_off(&Admin_led);
            led_turn_off(&Guest_led);
            lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
            lcd_8bit_send_string(&LCD,"Session Timeout");
            __delay_ms(1000);
        }
        while (login_mode == NO_MODE)
        {
            if(block_mode_flag==TRUE)
            {
                lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
                lcd_8bit_send_string(&LCD, "Login blocked");
                lcd_8bit_send_string_pos(&LCD, "wait 20 seconds", 2,1);
                led_turn_on(&Block_led);
                __delay_ms(BLOCK_MODE_TIME);
                led_turn_off(&Block_led);
                pass_tries_count = 0;
                block_mode_flag = FALSE;
                EEPROM_WriteByte(LOGIN_BLOCKED_ADDRESS, FALSE); //Write false at blocked location in EEPROM
            }
            lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
            lcd_8bit_send_string(&LCD, "Select mode:");
            lcd_8bit_send_string_pos(&LCD, "0:Admin 1:Guest", 2,1);
            keypad_value = NO_KEY_PRESSED;
            while(keypad_value == NO_KEY_PRESSED)
            {
                keypad_value = keypad_get_value(&keypad);
            }
            if((keypad_value != ADMIN_MODE) && (keypad_value != GUEST_MODE))
			{
				lcd_8bit_send_cmd(&LCD, LCD_CLEAR);//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
				lcd_8bit_send_string(&LCD, "Wrong input.");//Prints error message on the LCD
				keypad_value = NO_KEY_PRESSED;//return the variable that holds the pressed key from keypad to its initial value
				__delay_ms(1000);//Halt the system for the given time in (ms)
				continue;//return to the loop of login #while (login_mode==NO_MODE)# line 121
			}
            uint8 password_counter = 0;//counts the entered key of the password from the keypad
			uint8 password[PASS_SIZE] = {NOT_STORED,NOT_STORED,NOT_STORED,NOT_STORED};//temporarily hold the entire password that will be entered by the user to be checked
			uint8 stored_password[PASS_SIZE] = {NOT_STORED,NOT_STORED,NOT_STORED,NOT_STORED};//temporarily hold the entire stored password that is written by the user for the first time
            
            switch(keypad_value)
			{
                case ADMIN_MODE:
                    while(login_mode != ADMIN)
                    {
                        lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
                        lcd_8bit_send_string(&LCD, "Admin mode");
                        lcd_8bit_send_string_pos(&LCD, "Enter Pass:", 2,1);
                        __delay_ms(200);
                        password_counter = 0;
                        while(password_counter < PASS_SIZE)
                        {
                            keypad_value = NO_KEY_PRESSED;
                            while(keypad_value == NO_KEY_PRESSED)
                            {
                                keypad_value = keypad_get_value(&keypad);
                            }
                            password[password_counter] = keypad_value;//add the pressed character to the pass array
                            lcd_8bit_send_char(&LCD, keypad_value);
                            __delay_ms(CHARACTER_PREVIEW_TIME);//Halt the system for the given time in (ms)
                            lcd_8bit_send_char_pos(&LCD, PASSWORD_SYMBOL, 2, 12+password_counter);
                            __delay_ms(50);//Halt the system for the given time in (ms)
                            password_counter++;//increase the characters count
                        }
                        EEPROM_ReadBlock(EEPROM_ADMIN_ADDRESS, stored_password, PASS_SIZE);
                        /*compare passwords*/
                        if((ComparePass(password, stored_password, PASS_SIZE)) == TRUE)//in case of right password
                        {
                            login_mode = ADMIN;
                            pass_tries_count = 0;//clear the counter of wrong tries
                            lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
                            lcd_8bit_send_string(&LCD, "Right password");
                            lcd_8bit_send_string_pos(&LCD, "Admin mode", 2,1);
                            __delay_ms(500);
                            led_turn_on(&Admin_led);
                            Timer0_Init(&timer);
                            lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
                        }
                        else
                        {
                            pass_tries_count++;//increase the number of wrong tries to block login if it exceeds the allowed tries
                            login_mode = NO_MODE;//set the mode as not logged in
                            lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
                            lcd_8bit_send_string(&LCD, "Wrong password");
                            lcd_8bit_send_string_pos(&LCD, "Tries left:", 2,1);
                            lcd_8bit_send_char(&LCD, TRIES_ALLOWED-pass_tries_count+48);
                            __delay_ms(1000);
                            lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
                            if (pass_tries_count>=TRIES_ALLOWED)//if the condition of the block mode is true
                            {
                                block_mode_flag = TRUE;//turn on block mode
                                EEPROM_WriteByte(LOGIN_BLOCKED_ADDRESS,block_mode_flag);//write to the EEPROM TRUE to the the block mode address
                                break;//break the loop of admin login #while(login_mode!=GUEST)# at line 158
                            }
                        } 
                    }
                    break;
                /********************************* Guest login **********************************************/
                case GUEST_MODE:
                   while(login_mode != GUEST)
                    {
                        lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
                        lcd_8bit_send_string(&LCD, "Guest mode");
                        lcd_8bit_send_string_pos(&LCD, "Enter Pass:", 2,1);
                        __delay_ms(200);
                        password_counter = 0;
                        while(password_counter < PASS_SIZE)
                        {
                            keypad_value = NO_KEY_PRESSED;
                            while(keypad_value == NO_KEY_PRESSED)
                            {
                                keypad_value = keypad_get_value(&keypad);
                            }
                            password[password_counter] = keypad_value;//add the pressed character to the pass array
                            lcd_8bit_send_char(&LCD, keypad_value);
                            __delay_ms(CHARACTER_PREVIEW_TIME);//Halt the system for the given time in (ms)
                            lcd_8bit_send_char_pos(&LCD, PASSWORD_SYMBOL, 2, 12+password_counter);
                            __delay_ms(50);//Halt the system for the given time in (ms)
                            password_counter++;//increase the characters count
                        }
                        EEPROM_ReadBlock(EEPROM_GUEST_ADDRESS, stored_password, PASS_SIZE);
                        /*compare passwords*/
                        if((ComparePass(password, stored_password, PASS_SIZE)) == TRUE)//in case of right password
                        {
                            login_mode = GUEST;
                            pass_tries_count = 0;//clear the counter of wrong tries
                            lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
                            lcd_8bit_send_string(&LCD, "Right password");
                            lcd_8bit_send_string_pos(&LCD, "Guest mode", 2,1);
                            __delay_ms(500);
                            led_turn_on(&Guest_led);
                            Timer0_Init(&timer);
                            lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
                        }
                        else
                        {
                            pass_tries_count++;//increase the number of wrong tries to block login if it exceeds the allowed tries
                            login_mode = NO_MODE;//set the mode as not logged in
                            lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
                            lcd_8bit_send_string(&LCD, "Wrong password");
                            lcd_8bit_send_string_pos(&LCD, "Tries left:", 2,1);
                            lcd_8bit_send_char(&LCD, TRIES_ALLOWED-pass_tries_count+48);
                            __delay_ms(1000);
                            lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
                            if (pass_tries_count>=TRIES_ALLOWED)//if the condition of the block mode is true
                            {
                                block_mode_flag = TRUE;//turn on block mode
                                EEPROM_WriteByte(LOGIN_BLOCKED_ADDRESS,block_mode_flag);//write to the EEPROM TRUE to the the block mode address
                                break;//break the loop of admin login #while(login_mode!=ADMIN)# at line 214
                            }
                        } 
                    } 
                    break;
                defualt:
                    break;
            }
        }
        /*************************************************************************************************/
		uint8 show_menu = MAIN_MENU;
        
        while(timeout_flag != TRUE)//Show the menu in case of the time is not out
		{
            switch (show_menu)
            {
                case MAIN_MENU:
                    do
                    {
                        lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
                        lcd_8bit_send_string(&LCD, "1:Room1 2:Room2");
                        if(login_mode == ADMIN)
                        {
                            lcd_8bit_send_string_pos(&LCD, "3:Room3 4:More", 2,1);
                        }
                        else if(login_mode==GUEST)
                        {
                            lcd_8bit_send_string_pos(&LCD, "3:Room3 4:Room4", 2,1);
                        }
                        keypad_value = GetKeyPressed(login_mode);
                        __delay_ms(50);//to avoid the duplication of the pressed key
                        
                        if(keypad_value == SELECT_ROOM1)//If key pressed is 1
                        {
                            show_menu = ROOM1_MENU;//Set the next menu to be shown to room1 menu
                        }
                        else if(keypad_value == SELECT_ROOM2)//If key pressed is 2
                        {
                            show_menu = ROOM2_MENU;//Set the next menu to be shown to room2 menu
                        }
                        else if(keypad_value == SELECT_ROOM3)//If key pressed is 3
                        {
                            show_menu = ROOM3_MENU;//Set the next menu to be shown to room3 menu
                        }
                        else if(keypad_value == SELECT_ROOM4 && login_mode == GUEST)//If key pressed is 4 and the logged in user is guest
                        {
                            show_menu = ROOM4_MENU;//Set the next menu to be shown to room4 menu
                        }
                        else if(keypad_value == ADMIN_MORE_OPTION && login_mode == ADMIN)//If key pressed is 4 and the logged in user is admin
                        {
                            show_menu = MORE_MENU;//Set the next menu to be shown to more menu
                        }
                        else if(keypad_value != NO_KEY_PRESSED)//show wrong input message if the user pressed wrong key
                        {
                            lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
                            lcd_8bit_send_string(&LCD, "Wrong input");
                            __delay_ms(500);//Halt the system for the given time in (ms)
                        }
                    }while(((keypad_value < '1') || (keypad_value > '4') ) && (timeout_flag == FALSE));
                    break;
                    
                case MORE_MENU:
                    do
                    {
                        lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
                        lcd_8bit_send_string(&LCD, "1:Room4 2:TV");
                        lcd_8bit_send_string_pos(&LCD, "3:Air Cond.4:RET", 2,1);
                        
                        keypad_value = GetKeyPressed(login_mode);
                        __delay_ms(50);//to avoid the duplication of the pressed key
                        
                        if(keypad_value == SELECT_ROOM4_ADMIN)
                        {
                            show_menu = ROOM4_MENU;
                        }
                        else if(keypad_value == SELECT_TV)
                        {
                            show_menu = TV_MENU;
                        }
                        else if(keypad_value == SELECT_AIR_CONDITIONING)
                        {
                            show_menu = AIRCONDITIONING_MENU;
                        }
                        else if(keypad_value == ADMIN_RET_OPTION)
                        {
                            show_menu = MAIN_MENU;
                        }
                        else if(keypad_value != NO_KEY_PRESSED)//show wrong input message if the user pressed wrong key
                        {
                            lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
                            lcd_8bit_send_string(&LCD, "Wrong input");
                            __delay_ms(500);//Halt the system for the given time in (ms)
                        }
                    }while(((keypad_value < '1') || (keypad_value > '4') ) && (timeout_flag == FALSE));
                    break;//End of more menu case
                    
                case AIRCONDITIONING_MENU:
                    do
                    {
                        lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
                        lcd_8bit_send_string(&LCD, "1:Set temperature");
                        lcd_8bit_send_string_pos(&LCD, "2:Control  0:RET", 2,1);

                        keypad_value = GetKeyPressed(login_mode);
                        __delay_ms(50);//to avoid the duplication of the pressed key
                        
                        if(keypad_value == SELECT_SET_TEMPERATURE)
                        {
                            show_menu = TEMPERATURE_MENU;
                        }
                        else if(keypad_value == SELECT_AIR_COND_CTRL)
                        {
                            show_menu = AIRCOND_CTRL_MENU;
                        }
                        else if(keypad_value == SELECT_AIR_COND_RET)
                        {
                            show_menu = MORE_MENU;
                        }
                        else if(keypad_value != NO_KEY_PRESSED)//show wrong input message if the user pressed wrong key
                        {
                            lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
                            lcd_8bit_send_string(&LCD, "Wrong input");
                            __delay_ms(500);//Halt the system for the given time in (ms)
                        }
                    }while(((keypad_value < '0') || (keypad_value > '2') ) && (timeout_flag == FALSE));
                    break;//End of air conditioning menu case
                    
                case ROOM1_MENU:
                    MenuOption(ROOM1_MENU,login_mode);//call the function that show the menu of room 1
                    show_menu = MAIN_MENU;//Set the next menu to be shown to main menu
                    break;
                case ROOM2_MENU:
                    MenuOption(ROOM2_MENU,login_mode);//call the function that show the menu of room 2
                    show_menu = MAIN_MENU;//Set the next menu to be shown to main menu
                    break;
                 case ROOM3_MENU:
                    MenuOption(ROOM3_MENU,login_mode);//call the function that show the menu of room 3
                    show_menu = MAIN_MENU;//Set the next menu to be shown to main menu
                    break;
                case ROOM4_MENU:
                    MenuOption(ROOM4_MENU,login_mode);//call the function that show the menu of room 4
                    if (login_mode == GUEST)//in case of guest is logged in
                    {
                        show_menu = MAIN_MENU;//Set the next menu to be shown to main menu
                    }
                    else//in case of admin is logged in
                    {
                        show_menu = MORE_MENU;//Set the next menu to be shown to more menu
                    }
                    break;//End of room4 menu case
                    
                case TV_MENU:
                    MenuOption(TV_MENU,login_mode);//call the function that show the menu of tv
                    show_menu = MORE_MENU;//Set the next menu to be shown to more menu
                    break;
                case AIRCOND_CTRL_MENU:
                    MenuOption(AIRCOND_CTRL_MENU,login_mode);//call the function that show the menu of Air conditioning control
                    show_menu = AIRCONDITIONING_MENU;//Set the next menu to be shown to air conditioning menu
                    break;    
    
                case TEMPERATURE_MENU:
                    temperature = 0;//clear the value of temperature
                    while (temperature==0 && timeout_flag == FALSE)//start the loop that asks for the temperature from the user in case of the time is not out
                    {
                        keypad_value = NO_KEY_PRESSED;
                        lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
                        lcd_8bit_send_string(&LCD, "Set temp.:__");
                        lcd_8bit_send_char(&LCD, DEGREES_SYMBOL);
                        lcd_8bit_send_char(&LCD, 'C');
                        //lcd_8bit_send_char_pos(&LCD, PASSWORD_SYMBOL, 2, 12+password_counter);
                        
                        keypad_value = GetKeyPressed(login_mode);//wait for the user till key is pressed or the time is out
                        __delay_ms(200);//to avoid the duplication of the pressed key
                        if(timeout_flag == TRUE) //in case of the time is out before the user press a key
                        {
                            break;//break the loop that ask for temperature
                        }
                        if(keypad_value <'0' || keypad_value >'9')//show wrong input message if the user entered non numeric value
                        {
                            lcd_8bit_send_cmd(&LCD, LCD_CLEAR);//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
                            lcd_8bit_send_string(&LCD, "Wrong input");//print error message
                            __delay_ms(500);//Halt the system for the given time in (ms)
                            continue;//return to #while (temperature==0)# line 672
                        }
                        else//if the value is valid
                        {
                            lcd_8bit_send_char_pos(&LCD, keypad_value, 1, 11);
                            temp_tens = keypad_value-ASCII_ZERO;//save the entered value
                            keypad_value = NO_KEY_PRESSED;//set the key pressed to the default value
                        }
                        /*******************************************************************************/   
                        keypad_value = GetKeyPressed(login_mode);
                        __delay_ms(200);//to avoid the duplication of the pressed key
                        
                        if(timeout_flag == TRUE) //in case of the time is out before the user press a key
                        {
                            break;//break the loop that ask for temperature
                        }
                        if(keypad_value <'0' || keypad_value >'9')//show wrong input message if the user entered non numeric value
                        {
                            lcd_8bit_send_cmd(&LCD, LCD_CLEAR);//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
                            lcd_8bit_send_string(&LCD, "Wrong input");//print error message
                            __delay_ms(500);//Halt the system for the given time in (ms)
                            continue;//return to #while (temperature==0)# line 672
                        }
                        else//if the value is valid
                        {
                            lcd_8bit_send_char(&LCD, keypad_value);
                            temp_ones = keypad_value-ASCII_ZERO;//save the entered value
                            keypad_value = NO_KEY_PRESSED;//set the key pressed to the default value
                        }
                        temperature = temp_tens*10 + temp_ones;
                        SPI_Transfer_data(SET_TEMPERATURE);//Send the code of set temperature
                        __delay_ms(200);//Halt the system to prevent write collision
                        SPI_Transfer_data(temperature);//Send the code of set temperature
                        lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
                        lcd_8bit_send_string(&LCD, "Temperature Sent");
                        __delay_ms(500);//Halt the system for the given time in (ms)
                    }
                    show_menu = AIRCONDITIONING_MENU;//Set the next menu to be shown to air conditioning menu
                    break;//break from switch 
            }//End of the switch
        }//End of while that repeats the menu after each successful action till session timeout
    }// end of the main while(1)
    return 0;
}

void TMR0_InterruptHandler(void)
{
    session_counter++;//increase the indicator of session time for every tick
}

uint8 ComparePass(const uint8* pass1,const uint8* pass2,const uint8 size)
{
    uint8 Compare_Counter = ZERO_INIT;
    uint8 retValue = TRUE;
    for(Compare_Counter = 0; Compare_Counter < size; Compare_Counter++)
    {
        if(pass1[Compare_Counter] != pass2[Compare_Counter])
        {
            retValue = FALSE;
            break;
        }
    }
    return retValue;
}

uint8 GetKeyPressed(const uint8 LoginMode)
{
    uint8 key_pressed = NO_KEY_PRESSED;
	while (key_pressed == NO_KEY_PRESSED)//repeat till the user press any key
	{
		if ((session_counter >= ADMIN_TIMEOUT && LoginMode == ADMIN) || (session_counter >= GUEST_TIMEOUT && LoginMode == GUEST))//check for timeout
		{
			timeout_flag = TRUE;//set timeout flag to true
			break;//break the loop that wait for input from the user
		}
		
		key_pressed = keypad_get_value(&keypad);;//if the user pressed any button in keypad save the value in key_pressed
	}
	return key_pressed;
}

void MenuOption(const uint8 SelectedRoom, const uint8 LoginMode)
{
    uint8 StatusCode  = 0;//demand status code
	uint8 TurnOnCode  = 0;//turn on the room or the device code
	uint8 TurnOffCode = 0;//turn off the device or room
	uint8 response    = DEFAULT_ACK;//the response of the slave that is sent back based on the command of the master
	uint8 key_pressed = NO_KEY_PRESSED;//the key that is entered by the user
    
    do
    {
        lcd_8bit_send_cmd(&LCD, LCD_CLEAR);
        switch(SelectedRoom)
		{
			case ROOM1_MENU:
			StatusCode = ROOM1_STATUS;
			TurnOnCode = ROOM1_TURN_ON;
			TurnOffCode = ROOM1_TURN_OFF;
			lcd_8bit_send_string(&LCD, "Room1 S:");
			break;
			case ROOM2_MENU:
			StatusCode = ROOM2_STATUS;
			TurnOnCode = ROOM2_TURN_ON;
			TurnOffCode = ROOM2_TURN_OFF;
			lcd_8bit_send_string(&LCD, "Room2 S:");
			break;
			case ROOM3_MENU:
			StatusCode = ROOM3_STATUS;
			TurnOnCode = ROOM3_TURN_ON;
			TurnOffCode = ROOM3_TURN_OFF;
			lcd_8bit_send_string(&LCD, "Room3 S:");
			break;
			case ROOM4_MENU:
			StatusCode = ROOM4_STATUS;
			TurnOnCode = ROOM4_TURN_ON;
			TurnOffCode = ROOM4_TURN_OFF;
			lcd_8bit_send_string(&LCD, "Room4 S:");
			break;
			case TV_MENU:
			StatusCode = TV_STATUS;
			TurnOnCode = TV_TURN_ON;
			TurnOffCode = TV_TURN_OFF;
			lcd_8bit_send_string(&LCD, "TV S:");
			break;
			case AIRCOND_CTRL_MENU:
			StatusCode = AIR_COND_STATUS;
			TurnOnCode = AIR_COND_TURN_ON;
			TurnOffCode = AIR_COND_TURN_OFF;
			lcd_8bit_send_string(&LCD, "Air Cond. S:");
			break;
		}
        /****************************************************************************************************/
        SPI_Transfer_data(StatusCode);
        __delay_ms(100);//Halt the system for the given time in (ms)
        response = SPI_Transfer_data(DEMAND_RESPONSE);
        
        if(response == ON_STATUS)//if the response from the slave was on status
		{
			lcd_8bit_send_string(&LCD, "ON");
		}
		else//if the response from the slave was off status
		{
			lcd_8bit_send_string(&LCD, "OFF");
		}
        
        lcd_8bit_send_string_pos(&LCD, "1-On 2-Off 0-RET", 2,1);
        key_pressed = GetKeyPressed(LoginMode);
        /*there is no need to take any action in case of the user pressed 0(RET) key
		breaking the loop will be enough since it will be handled in the main*/
        if (key_pressed == '1')
		{
			SPI_Transfer_data(TurnOnCode);//Send turn on signal from master to slave
		}
		else if (key_pressed == '2')
		{
			SPI_Transfer_data(TurnOffCode);//Send turn off signal from master to slave
		}
		else if( (key_pressed != NO_KEY_PRESSED) && (key_pressed != '0') )//show wrong input message if the user entered non numeric value
		{
			lcd_8bit_send_cmd(&LCD, LCD_CLEAR);//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
			lcd_8bit_send_string(&LCD, "Wrong input");//print error message
			__delay_ms(500);//Halt the system for the given time in (ms)
		}
    }while(((key_pressed < '0') || (key_pressed > '2')) && (timeout_flag == FALSE));
}