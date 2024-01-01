/* 
 * File:   Master_App.h
 * Author: Mohamed Sameh
 *
 * Created on December 27, 2023, 11:21 PM
 */

#ifndef MASTER_APP_H
#define	MASTER_APP_H

/* Section : Includes */
#include "Init_layer.h"


/* Section : Macro Declarations */
#define NOT_STORED   0xFF
#define NOT_SELECTED 0xFF

#define BLOCK_MODE_TIME		   (uint32)20000
#define CHARACTER_PREVIEW_TIME (uint16)500
#define DEGREES_SYMBOL		   (uint8)0xDF

/************************************ Login configurations *****************************/
#define NO_MODE (uint8)0
#define ADMIN   (uint8)1
#define GUEST   (uint8)2

/************************************ Logic values *************************************/
#define FALSE   (uint8)0
#define TRUE    (uint8)1

/*********************************** PIN Configuration ***********************************/
#define PASS_SIZE       (uint8)4
#define TRIES_ALLOWED   (uint8)3
#define PASSWORD_SYMBOL (uint8)'*'
#define ASCII_ZERO      (uint8)'0'

#define PASS_NOT_SET (uint8)0xFF
#define PASS_SET     (uint8)0x01

#define ADMIN_PASS_STATUS_ADDRESS (uint16)0X10
#define GUEST_PASS_STATUS_ADDRESS (uint16)0X11
#define EEPROM_ADMIN_ADDRESS      (uint16)0X12
#define EEPROM_GUEST_ADDRESS      (uint16)0X16
#define LOGIN_BLOCKED_ADDRESS     (uint16)0X28
/****************************   number of ticks to run timeout ***************************/
#define ADMIN_TIMEOUT (uint16)6000
#define GUEST_TIMEOUT (uint16)3000

/************************************ imp checks *************************************/
#define ADMIN_MODE        (uint8)'0'
#define GUEST_MODE        (uint8)'1'

#define SELECT_ROOM1            (uint8)'1'
#define SELECT_ROOM2            (uint8)'2'
#define SELECT_ROOM3            (uint8)'3'
#define SELECT_ROOM4            (uint8)'4'
#define ADMIN_MORE_OPTION       (uint8)'4'

#define SELECT_ROOM4_ADMIN      (uint8)'1'
#define SELECT_TV               (uint8)'2'
#define SELECT_AIR_CONDITIONING (uint8)'3'
#define ADMIN_RET_OPTION        (uint8)'4'

#define SELECT_SET_TEMPERATURE  (uint8)'1'
#define SELECT_AIR_COND_CTRL    (uint8)'2'
#define SELECT_AIR_COND_RET     (uint8)'0'

/****************************   Show menu codes  *****************************************/
#define MAIN_MENU            (uint8)1
#define MORE_MENU            (uint8)2
#define ROOM1_MENU           (uint8)3
#define ROOM2_MENU           (uint8)4
#define ROOM3_MENU           (uint8)5
#define ROOM4_MENU           (uint8)6
#define TV_MENU              (uint8)7
#define AIRCONDITIONING_MENU (uint8)8
#define AIRCOND_CTRL_MENU    (uint8)9
#define TEMPERATURE_MENU     (uint8)10

/****************************   Std msgs  *****************************************/
#define ROOM1_STATUS    0x11
#define ROOM2_STATUS    0x12
#define ROOM3_STATUS    0x13
#define ROOM4_STATUS    0x14
#define TV_STATUS 		0x15
#define AIR_COND_STATUS 0x16

#define ROOM1_TURN_ON    0x21
#define ROOM2_TURN_ON    0x22
#define ROOM3_TURN_ON    0x23
#define ROOM4_TURN_ON    0x24
#define TV_TURN_ON 		 0x25
#define AIR_COND_TURN_ON 0x26

#define ROOM1_TURN_OFF    0x31
#define ROOM2_TURN_OFF    0x32
#define ROOM3_TURN_OFF    0x33
#define ROOM4_TURN_OFF    0x34
#define TV_TURN_OFF 	  0x35
#define AIR_COND_TURN_OFF 0x36

#define SET_TEMPERATURE 0x40

#define DEFAULT_ACK    0xFF
#define DEMAND_RESPONSE 0xFF

#define ON_STATUS   0x01
#define OFF_STATUS  0x00
/* Section : Macro Functions Declarations */


/* Section : Data Types Declarations  */
extern keypad_t keypad;
extern lcd_8bit_t LCD;
extern led_t Admin_led;
extern led_t Guest_led;
extern led_t Block_led;
extern timer0_t timer;
extern spi_t spi;
/* Section : Functions Declarations */
void TMR0_InterruptHandler(void);
uint8 ComparePass(const uint8* pass1,const uint8* pass2,const uint8 size);
uint8 GetKeyPressed(const uint8 u8LoginMode);
void MenuOption(const uint8 SelectedRoom,const uint8 LoginMode);

#endif	/* MASTER_APP_H */

