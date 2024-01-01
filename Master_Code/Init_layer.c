/* 
 * File:   Init_layer.c
 * Author: Mohamed Sameh
 *
 * Created on December 27, 2023, 11:54 PM
 */

#include "Init_layer.h"

Std_ReturnType ret = E_NOT_OK;

led_t Admin_led = {.led_status = LED_OFF, .pin = GPIO_PIN0, .port = PORTC_INDEX};
led_t Guest_led = {.led_status = LED_OFF, .pin = GPIO_PIN1, .port = PORTC_INDEX};
led_t Block_led = {.led_status = LED_OFF, .pin = GPIO_PIN2, .port = PORTC_INDEX};

spi_t spi = 
{
    .mode = SPI_MASTER_FOSC_DIV_4,
    .master_waveform = SPI_CLK_IDLE_LOW_TX_LEADING_RISING,
    .master_sample = SPI_MASTER_SAMPLE_END_CFG,
    //.SPI_InterruptHandler = NULL
};
timer0_t timer = 
{
    .timer0_preload = 55536,
    .prescaler_status = TIMER0_PRESCALER_DISABLE_CFG,
    .timer0_mode = TIMER0_TIMER_MODE,
    .TMR0_InterruptHandler = TMR0_InterruptHandler,
};
keypad_t keypad = {
    .keypad_rows_pins[0].port = PORTD_INDEX,
    .keypad_rows_pins[0].pin_num = GPIO_PIN0,
    .keypad_rows_pins[0].direction = GPIO_DIRECTION_OUTPUT,
    .keypad_rows_pins[0].logic = GPIO_LOW,
     
    .keypad_rows_pins[1].port = PORTD_INDEX,
    .keypad_rows_pins[1].pin_num = GPIO_PIN1,
    .keypad_rows_pins[1].direction = GPIO_DIRECTION_OUTPUT,
    .keypad_rows_pins[1].logic = GPIO_LOW,
    
    .keypad_rows_pins[2].port = PORTD_INDEX,
    .keypad_rows_pins[2].pin_num = GPIO_PIN2,
    .keypad_rows_pins[2].direction = GPIO_DIRECTION_OUTPUT,
    .keypad_rows_pins[2].logic = GPIO_LOW,
    
    .keypad_rows_pins[3].port = PORTD_INDEX,
    .keypad_rows_pins[3].pin_num = GPIO_PIN3,
    .keypad_rows_pins[3].direction = GPIO_DIRECTION_OUTPUT,
    .keypad_rows_pins[3].logic = GPIO_LOW,    
    
    .keypad_columns_pins[0].port = PORTD_INDEX,
    .keypad_columns_pins[0].pin_num = GPIO_PIN4,
    .keypad_columns_pins[0].direction = GPIO_DIRECTION_INPUT,
    .keypad_columns_pins[0].logic = GPIO_LOW,
    
    .keypad_columns_pins[1].port = PORTD_INDEX,
    .keypad_columns_pins[1].pin_num = GPIO_PIN5,
    .keypad_columns_pins[1].direction = GPIO_DIRECTION_INPUT,
    .keypad_columns_pins[1].logic = GPIO_LOW,
    
    .keypad_columns_pins[2].port = PORTD_INDEX,
    .keypad_columns_pins[2].pin_num = GPIO_PIN6,
    .keypad_columns_pins[2].direction = GPIO_DIRECTION_INPUT,
    .keypad_columns_pins[2].logic = GPIO_LOW,
    
    .keypad_columns_pins[3].port = PORTD_INDEX,
    .keypad_columns_pins[3].pin_num = GPIO_PIN7,
    .keypad_columns_pins[3].direction = GPIO_DIRECTION_INPUT,
    .keypad_columns_pins[3].logic = GPIO_LOW,
};

lcd_8bit_t LCD = 
{
    .lcd_rs.port = PORTA_INDEX,
    .lcd_rs.pin_num = GPIO_PIN0,
    .lcd_rs.direction = GPIO_DIRECTION_OUTPUT,
    .lcd_rs.logic = GPIO_LOW,
    
    .lcd_en.port = PORTA_INDEX,
    .lcd_en.pin_num = GPIO_PIN1,
    .lcd_en.direction = GPIO_DIRECTION_OUTPUT,
    .lcd_en.logic = GPIO_LOW,
    
    .lcd_data[0].port = PORTB_INDEX,
    .lcd_data[0].pin_num = GPIO_PIN0,
    .lcd_data[0].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[0].logic = GPIO_LOW,
    
    .lcd_data[1].port = PORTB_INDEX,
    .lcd_data[1].pin_num = GPIO_PIN1,
    .lcd_data[1].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[1].logic = GPIO_LOW,
    
    .lcd_data[2].port = PORTB_INDEX,
    .lcd_data[2].pin_num = GPIO_PIN2,
    .lcd_data[2].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[2].logic = GPIO_LOW,
    
    .lcd_data[3].port = PORTB_INDEX,
    .lcd_data[3].pin_num = GPIO_PIN3,
    .lcd_data[3].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[3].logic = GPIO_LOW,
    
    .lcd_data[4].port = PORTB_INDEX,
    .lcd_data[4].pin_num = GPIO_PIN4,
    .lcd_data[4].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[4].logic = GPIO_LOW,
    
    .lcd_data[5].port = PORTB_INDEX,
    .lcd_data[5].pin_num = GPIO_PIN5,
    .lcd_data[5].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[5].logic = GPIO_LOW,
   
    .lcd_data[6].port = PORTB_INDEX,
    .lcd_data[6].pin_num = GPIO_PIN6,
    .lcd_data[6].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[6].logic = GPIO_LOW,
    
    .lcd_data[7].port = PORTB_INDEX,
    .lcd_data[7].pin_num = GPIO_PIN7,
    .lcd_data[7].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[7].logic = GPIO_LOW,
};
        
void application_init()
{
   ret = keypad_init(&keypad);
   ret = lcd_8bit_init(&LCD);
   
   ret = led_init(&Admin_led);
   ret = led_init(&Guest_led);
   ret = led_init(&Block_led);
   ret = SPI_Master_Init(&spi);
}