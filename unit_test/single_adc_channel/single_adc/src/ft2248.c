

#include <stdint.h>
#include "ft2248.h"




sensor_t * new_ft2248(uint8_t address){	

	sensor_t * sensor = NULL;
	sensor = new_sensor(LTC2448_SENSOR, address);
	
	//Set the channel count to the sensor type
	sensor->channel_count = LTC2448_SENSOR_CHANNEL_COUNT;
	sensor->data = (int32_t *) calloc(LTC2448_SENSOR_CHANNEL_COUNT, sizeof(int32_t));

	sensor->update_all_sensor_data_fp = NULL;
	sensor->update_sensor_data_fp = NULL;
	sensor->get_sensor_data_fp = NULL;
	sensor->get_all_sensor_data_fp = NULL;
	sensor->set_sensor_data_fp = NULL;
	sensor->set_all_sensor_data_fp = NULL;
	
	return sensor;
}

void delete_ft2248(sensor_t * sensor){
	free(sensor->data);
	delete_sensor(sensor);
}

