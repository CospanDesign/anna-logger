

#include <stdint.h>
#include "ft2248.h"




sensor_type_t * new_ft2248(uint8_t address){	

	sensor_type_t * sensor = NULL;
	sensor = new_sensor(LTC2448_SENSOR, address);
	
	//Set the channel count to the sensor type
	sensor->channel_count = LTC2448_SENSOR_CHANNEL_COUNT;
	sensor->data = (int32_t *) calloc(LTC2448_SENSOR_CHANNEL_COUNT, sizeof(int32_t));

	return sensor;
}

void delete_ft2248(sensor_type_t * sensor){
	free(sensor->data);
	delete_sensor(sensor);
}

