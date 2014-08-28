/*
 * LTC2448.c
 *
 * Created: 8/28/2014 12:25:55 PM
 *  Author: cospan
 */ 

#include "sensor.h"
#include <malloc.h>

sensor_type_t * new_sensor(uint8_t type, uint8_t address){
	//Local Function Declaration
	sensor_type_t * sensor = NULL;

	//Create some space for a sensor
	sensor = (sensor_type_t*) malloc(sizeof(sensor_type_t));
	if (sensor == NULL){
		return NULL;
	}
	
	//Set up the sensor data structure with all the constants
	sensor->type = type;
	sensor->address = address;
	
	
	return sensor;
}

void delete_sensor(sensor_type_t *sensor){
	free(sensor);	
};

void update_all_sensor_data(sensor_type_t *sensor){
}
uint32_t update_sensor_data(sensor_type_t *sensor, uint8_t channel){
	return 0;
}

uint8_t get_max_sensor_channel_count(sensor_type_t *sensor){
	return 0;
}
uint8_t get_sensor_type_t(sensor_type_t *sensor){
	return 0;
}
uint8_t get_sensor_address(sensor_type_t *sensor){
	return 0;
}

