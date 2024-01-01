/* 
 * File:   Init_layer.h
 * Author: Mohamed Sameh
 *
 * Created on December 31, 2023, 2:43 PM
 */

#ifndef INIT_LAYER_H
#define	INIT_LAYER_H

/* Section : Includes */
#include "HAL/LED/led.h"
#include "MCAL/interrupt/internal_interrupt.h"
#include "MCAL/TIMER0/timer0.h"
#include "MCAL/SPI/spi.h"
#include "MCAL/ADC/adc.h"

/* Section : Macro Declarations */

/* Section : Macro Functions Declarations */


/* Section : Data Types Declarations  */


/* Section : Functions Declarations */
void application_init();
extern void TMR0_InterruptHandler(void);

#endif	/* INIT_LAYER_H */

