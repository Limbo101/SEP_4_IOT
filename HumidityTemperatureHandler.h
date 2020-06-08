
/*
 * HumidityTemperatureHandler.h
 *
 * Created: 23.5.2020 г. 12:10:17
 *  Author: Borislav Aleksiev
 */ 
void hum_temp_task( void *pvParameters );

void Hum_Temp_Handler_create();
uint16_t get_hum_lvl();
uint16_t get_temp_lvl();
