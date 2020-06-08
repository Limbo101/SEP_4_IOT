
/*
 * CO2Handler.c
 *
 * Created: 23.5.2020 г. 10:49:34
 *  Author: Borislav Aleksiev, Karolis Gudynas, Dziugas Austys
 */ 

#include <stddef.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>

#include <lora_driver.h>
#include <iled.h>
#include "bits.h"
#include "event_groups.h"
#include "mh_z19.h"

uint16_t CO2level;
mh_z19_return_code_t rc;
	
void callbackFunction(uint16_t ppm);
void CO2_handler_task( void *pvParameters );

void CO2Handler_create()
{
	CO2level = (uint16_t)pvPortMalloc(sizeof(uint16_t));
	
	mh_z19_create(ser_USART3, callbackFunction);
	
	xTaskCreate(
		CO2_handler_task,
		(const portCHAR *)"CO2HandTask",  
		configMINIMAL_STACK_SIZE+200,
		NULL,
		3,
		NULL 
	);
}

void callbackFunction(uint16_t ppm){
	CO2level = ppm;	
}


uint16_t getCO2lvl(){ 
	return CO2level;
}


void CO2_handler_task( void *pvParameters )
{
	for (;;) {
		rc = mh_z19_take_meassuring();
		if (rc != MHZ19_OK)
		{
			printf("%s\n","Arduino measurement taking from sensor went wrong!");
		}
		printf("%s%i\n","Arduino measurement CO2: ", (int)CO2level);
		vTaskDelay(200);
	}
}

//mh_z19_return_code_t