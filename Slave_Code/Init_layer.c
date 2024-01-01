/* 
 * File:   Init_layer.c
 * Author: Mohamed Sameh
 *
 * Created on December 31, 2023, 2:43 PM
 */

#include "Init_layer.h"


Std_ReturnType ret = E_NOT_OK;

led_t Room1_led = {.led_status = LED_OFF, .pin = GPIO_PIN0, .port = PORTB_INDEX};
led_t Room2_led = {.led_status = LED_OFF, .pin = GPIO_PIN1, .port = PORTB_INDEX};
led_t Room3_led = {.led_status = LED_OFF, .pin = GPIO_PIN2, .port = PORTB_INDEX};
led_t Room4_led = {.led_status = LED_OFF, .pin = GPIO_PIN3, .port = PORTB_INDEX};

led_t TV_led       = {.led_status = LED_OFF, .pin = GPIO_PIN4, .port = PORTB_INDEX};
led_t Air_Cond_led = {.led_status = LED_OFF, .pin = GPIO_PIN5, .port = PORTB_INDEX};


adc_config_t adc0 = 
{
    //.ADC_InterruptHandler = NULL,
    .acq_time = ADC_0_TAD,
    .channel = ADC_CHANNEL_AN0,
    .res_format = ADC_RESULT_RIGHT,
    .clock = ADC_CLOCK_FOSC_DIV_2,
    .volt_reference = ADC_VOLT_REF_DISABLE
};
spi_t spi = 
{
    .mode = SPI_SLAVE_SS_DISABLED,
    //.SPI_InterruptHandler = NULL
};
timer0_t timer = 
{
    .timer0_preload = 55536,
    .prescaler_status = TIMER0_PRESCALER_DISABLE_CFG,
    .timer0_mode = TIMER0_TIMER_MODE,
    .TMR0_InterruptHandler = TMR0_InterruptHandler,
};
       
void application_init()
{
   
   ret = led_init(&Room1_led);
   ret = led_init(&Room2_led);
   ret = led_init(&Room3_led);
   ret = led_init(&Room4_led);
   ret = led_init(&TV_led);
   ret = led_init(&Air_Cond_led);
   
   ret = SPI_Slave_Init(&spi);
   ret = ADC_Init(&adc0);
}
