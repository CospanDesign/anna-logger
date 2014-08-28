/*
 * LTC2448.c
 *
 * Created: 8/28/2014 12:25:55 PM
 *  Author: cospan
 */ 

#include "sensor.h"


SensorBase::SensorBase(uint8_t type, uint8_t channel_count, uint8_t address){
	this->type = type;
	this->channel_count = channel_count;
	this->address = address;
}
SensorBase::~SensorBase(){
}

SensorBase::get_channel_cout(){
	return this->channel_count;	
}
SensorBase::get_type(){
	return this->type;
}
SensorBase::get_address(){
	return this->address;	
}

/*

sensor_type * new_sensor_struct(uint8_t type, uint8_t address){
	//Local Function Declaration
	sensor_type * sensor = NULL;

	//Create some space for a sensor
	sensor = (sensor_type*) malloc(sizeof(sensor_type));
	if (sensor == NULL){
		return NULL;
	}
	
	//Set up the sensor data structure with all the constants
	sensor->type = type;
	sensor->address = address;
	
	
	//Depending on the type of sensor create/remove certain pieces of data	
	switch (sensor->type){
		case(LTC2448_SENSOR):
			sensor->channel_count = LTC2448_SENSOR_CHANNEL_COUNT;
			sensor->data = (uint32_t *) calloc(LTC2448_SENSOR_CHANNEL_COUNT, sizeof(uint32_t));
		case(THERMAL_SENSOR):
			sensor->channel_count = THERMAL_SENSOR_CHANNEL_COUNT;
			sensor->data = (uint32_t *) calloc(THERMAL_SENSOR_CHANNEL_COUNT, sizeof(uint32_t));
		case(POWER_SUPPLY_SENSOR):
			sensor->channel_count = POWER_SUPPLY_CHANNEL_COUNT;
			sensor->data = (power_supply_data_type *) calloc(POWER_SUPPLY_CHANNEL_COUNT, sizeof(power_supply_data_type));
		default:
			sensor->channel_count = 0;
			sensor->data = NULL;
	}
	return sensor;
}

void delete_sensor_struct(sensor_type *sensor){
	free(sensor->data);
	free(sensor);	
};

uint32_t get_sensor_data(sensor_type *sensor, uint8_t channel){
	return 0;
}
void update_all_sensor_data(sensor_type *sensor){
}
uint32_t update_sensor_data(sensor_type *sensor, uint8_t channel){
	return 0;
}

uint8_t get_max_sensor_channel_count(sensor_type *sensor){
	return 0;
}
uint8_t get_sensor_type(sensor_type *sensor){
	return 0;
}
uint8_t get_sensor_address(sensor_type *sensor){
	return 0;
}

*/