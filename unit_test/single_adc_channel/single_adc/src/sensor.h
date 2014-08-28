#ifndef __SENSOR_H__
#define __SENSOR_H__

#include <stdint.h>
#include <malloc.h>


struct _sensor_type_t{
	uint8_t type;
	uint8_t address;
	uint8_t channel_count;
		
	void * data;
};

typedef struct _sensor_type_t sensor_type_t;

sensor_type_t * new_sensor(uint8_t type, uint8_t address);
void delete_sensor(sensor_type_t *sensor);
void update_all_sensor_data(sensor_type_t *sensor);
uint32_t update_sensor_data(sensor_type_t *sensor, uint8_t channel);
uint8_t get_max_sensor_channel_count(sensor_type_t *sensor);
uint8_t get_sensor_type_t(sensor_type_t *sensor);
uint8_t get_sensor_address(sensor_type_t *sensor);

#endif