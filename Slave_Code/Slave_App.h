/* 
 * File:   Slave_App.h
 * Author: Mohamed Sameh
 *
 * Created on December 31, 2023, 2:40 PM
 */

#ifndef SLAVE_APP_H
#define	SLAVE_APP_H

/* Section : Includes */
#include "Init_layer.h"

/* Section : Macro Declarations */
#define AIR_CONDTIONING_ON          (uint8)0x01
#define AIR_CONDTIONING_OFF			(uint8)0x00

#define AIR_COND_PIN				(uint8)2
#define TV_PIN						(uint8)3
#define ROOM1_PIN					(uint8)4
#define ROOM2_PIN					(uint8)5
#define ROOM3_PIN					(uint8)6
#define ROOM4_PIN					(uint8)7

#define AIR_COND_PORT 				(uint8)'D'
#define TV_PORT       				(uint8)'D'
#define ROOM1_PORT    				(uint8)'D'
#define ROOM2_PORT   				(uint8)'D'
#define ROOM3_PORT   				(uint8)'D'
#define ROOM4_PORT    				(uint8)'D'

#define ADC_STEP                    4.88f
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
extern led_t Room1_led;
extern led_t Room2_led;
extern led_t Room3_led;
extern led_t Room4_led;
extern led_t TV_led;
extern led_t Air_Cond_led;

extern spi_t spi;
extern timer0_t timer;
extern adc_config_t adc0;

/* Section : Functions Declarations */
void TMR0_InterruptHandler(void);
#endif	/* SLAVE_APP_H */

