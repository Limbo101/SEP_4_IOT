
/*
 * CO2Handler.c
 *
 * Created: 23.5.2020 г. 10:49:34
 *  Author: Borislav Aleksiev
 */ 
#include <stddef.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>

#include <lora_driver.h>
#include <iled.h>
#include "bits.h"
#include "event_groups.h"
#include "hih8120.h"

float temperature;
float humidity;
int ret;

	
void hum_temp_task(void *pvParameters);

void Hum_Temp_Handler_create()
{
	hih8120Create(); 
	
	xTaskCreate(
		hum_temp_task,
		(const portCHAR *)"HumTempTask",  // A name just for humans
		configMINIMAL_STACK_SIZE+200,  // This stack size can be checked & adjusted by reading the Stack Highwater
		NULL,
		3,  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
		NULL
	 );
}

uint16_t get_hum_lvl()
{
	while(true) // wait till the sensor is ready
	{
		ret = hih8120IsReady();
		if (ret) break;
		vTaskDelay(100);
	}
	
	humidity = hih8120GetHumidity();
	
	printf("%s%i%s\n", "Arduino humidity level: ", (int)humidity, "%");
	
	return humidity;
}


int16_t get_temp_lvl()
{ // old code was disgusting :<
	while(true) // wait till the sensor is ready
	{
		ret = hih8120IsReady();
		if (ret) break;
		vTaskDelay(100);
	}
	
	temperature = hih8120GetTemperature();
	
	printf("%s%i%s\n","Arduino temperature : ", (int)temperature, "C");
	
	return temperature;
}
void hum_temp_task( void *pvParameters )
{
	for (;;) 
	{
		
		ret = hih8120Wakeup();
		if (HIH8120_OK != ret)
			printf("%s", "Something went wrong with HIH8120 wakeup call! Return value was: ", ret);

		vTaskDelay(60); // necessary wait delay after calling wakeup
		
		ret = hih8120Meassure();
		if (HIH8120_OK != ret)
			printf("%s", "Something went wrong with HIH8120 Measure call! Return value was: ", ret);
			
		vTaskDelay(150);
		
		get_temp_lvl();
		get_hum_lvl();
			
		
	}
	
}
