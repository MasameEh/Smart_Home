/* 
 * File:   Slave_App.c
 * Author: Mohamed Sameh
 *
 * Created on December 31, 2023, 2:40 PM
 */
#include "Slave_App.h"

volatile uint16 required_temperature = 24; // the required temperature which sent from Master with initial value 24
volatile uint16 adc_res = 0; // the temperature of the room 
volatile uint16 temp_sensor_reading = 0; // the temperature of the room 
volatile uint8 counter = 0; // the counter which determine the periodic time of implementing ISR
volatile uint8 last_air_conditioning_value = AIR_CONDTIONING_OFF; // last air conditioning value which will help in hysteresis

int main()
{
    /*****************  INITIALIZE  ***********************/
    application_init();
    
    uint8 request = DEFAULT_ACK;//the value that is received from the master
	uint8 response = DEFAULT_ACK;//the values that is sent back to the master
    logic_t led_logic = 0xFF;
    
    while(1)
    {
        request = SPI_Transfer_data(DEFAULT_ACK);
        switch (request)
        {
            /*********************************   STATUS COMMANDS ********************************/
			//commands related to send the current status back to the master
			case ROOM1_STATUS:
                led_read(&Room1_led, &led_logic);
                if(GPIO_LOW == led_logic)
                {
                    response = OFF_STATUS;//set the response as off status
                }
                else if(GPIO_HIGH == led_logic)
                {
                    response = ON_STATUS;//set the response as on status
                }else{/* Nothing */}     
                
                SPI_Transfer_data(response);         
                break;
                
            case ROOM2_STATUS:
                led_read(&Room2_led, &led_logic);
                if(GPIO_LOW == led_logic)
                {
                    response = OFF_STATUS;//set the response as off status
                }
                else if(GPIO_HIGH == led_logic)
                {
                    response = ON_STATUS;//set the response as on status
                }else{/* Nothing */}     
                
                SPI_Transfer_data(response);         
                break;
            
            case ROOM3_STATUS:
                led_read(&Room3_led, &led_logic);
                if(GPIO_LOW == led_logic)
                {
                    response = OFF_STATUS;//set the response as off status
                }
                else if(GPIO_HIGH == led_logic)
                {
                    response = ON_STATUS;//set the response as on status
                }else{/* Nothing */}     
                
                SPI_Transfer_data(response);         
                break;
                
            case ROOM4_STATUS:
                led_read(&Room4_led, &led_logic);
                if(GPIO_LOW == led_logic)
                {
                    response = OFF_STATUS;//set the response as off status
                }
                else if(GPIO_HIGH == led_logic)
                {
                    response = ON_STATUS;//set the response as on status
                }else{/* Nothing */}     
                
                SPI_Transfer_data(response);         
                break;
                
            case AIR_COND_STATUS:
                led_read(&Air_Cond_led, &led_logic);
                if(GPIO_LOW == led_logic)
                {
                    response = OFF_STATUS;//set the response as off status
                }
                else if(GPIO_HIGH == led_logic)
                {
                    response = ON_STATUS;//set the response as on status
                }else{/* Nothing */}     
                
                SPI_Transfer_data(response);
                break;
                
            case TV_STATUS:
                led_read(&TV_led, &led_logic);
                if(GPIO_LOW == led_logic)
                {
                    response = OFF_STATUS;//set the response as off status
                }
                else if(GPIO_HIGH == led_logic)
                {
                    response = ON_STATUS;//set the response as on status
                }else{/* Nothing */}     
                
                SPI_Transfer_data(response);
                break;
                
            /*********************************   TURN ON COMMANDS ********************************/
            case ROOM1_TURN_ON:
                led_turn_on(&Room1_led);//turn on the led of room 1
                break;//break the switch case
			case ROOM2_TURN_ON:
                led_turn_on(&Room2_led);//turn on the led of room 2
                break;//break the switch case
			case ROOM3_TURN_ON:
                led_turn_on(&Room3_led);//turn on the led of room 3
                break;//break the switch case
			case ROOM4_TURN_ON:
                led_turn_on(&Room4_led);//turn on the led of room 4
                break;//break the switch case
			case AIR_COND_TURN_ON:
                Timer0_Init(&timer);
                led_turn_on(&Air_Cond_led);//turn on the led of air conditioning
                break;//break the switch case
			case TV_TURN_ON:
                led_turn_on(&TV_led);//turn on the led of the TV
                break;//break the switch case
            /*********************************   TURN OFF COMMANDS ********************************/
            case ROOM1_TURN_OFF:
                led_turn_off(&Room1_led);//turn off the led of room 1
                break;//break the switch case
			case ROOM2_TURN_OFF:
                led_turn_off(&Room2_led);//turn off the led of room 2
                break;//break the switch case
			case ROOM3_TURN_OFF:
                led_turn_off(&Room3_led);//turn off the led of room 3
                break;//break the switch case
			case ROOM4_TURN_OFF:
                led_turn_off(&Room4_led);//turn off the led of room 4
                break;//break the switch case
			case AIR_COND_TURN_OFF:
                Timer0_DeInit(&timer);
                led_turn_off(&Air_Cond_led);//turn off the led of air conditioning
                break;//break the switch case
			case TV_TURN_OFF:
                led_turn_off(&TV_led);//turn off the led of the TV
                break;//break the switch case
            /*********************************   Set temperature   ********************************/
            case SET_TEMPERATURE:
                required_temperature = SPI_Transfer_data(DEFAULT_ACK);
                break;
                
        }
        
    }
    return 0;
}

void TMR0_InterruptHandler(void)
{
    counter++;//count the ticks of the timer zero
    if(counter >= 10)//do that code every 10 ticks 
    {   
        counter = 0;//clear the counter of ticks
        ADC_Get_Conversion_Blocking(&adc0, ADC_CHANNEL_AN0,&adc_res);
        temp_sensor_reading = ADC_STEP * adc_res;
        temp_sensor_reading /= 10;
        if(temp_sensor_reading >= (required_temperature+1))//do that code if the read temperature if greater than required temperature by one or more
		{
			led_turn_on(&Air_Cond_led);//turn on the led of air conditioning
			last_air_conditioning_value = AIR_CONDTIONING_ON;//save the value of the state of the air conditioning
		}
        else if(temp_sensor_reading <= (required_temperature-1))
        {   
            led_turn_off(&Air_Cond_led);//turn off the led of air conditioning
            last_air_conditioning_value=AIR_CONDTIONING_OFF;//save the value of the state of the air conditioning
        }
        else if(required_temperature == temp_sensor_reading)//do that code if the read temperature is equal to the required temperature
		{
			if(last_air_conditioning_value == AIR_CONDTIONING_ON)//in the case of the last saved status of the air conditioning was on 
			{
				led_turn_on(&Air_Cond_led);//turn on the led of the air conditioning
			}
			else if(last_air_conditioning_value == AIR_CONDTIONING_OFF)//in the case of the last saved status of the air conditioning was off 
			{
				led_turn_off(&Air_Cond_led);//turn off the led of the air conditioning
			}
		}
    }
}