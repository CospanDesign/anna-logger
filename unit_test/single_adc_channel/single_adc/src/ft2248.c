
#include <stdio.h>
#include <stdint.h>
#include "ft2248.h"




sensor_t * new_ft2248(uint8_t address){	

	sensor_t * sensor = NULL;
	sensor = new_sensor(LTC2448_SENSOR, address);
	
	//Set the channel count to the sensor type
	sensor->channel_count = LTC2448_SENSOR_CHANNEL_COUNT;
	sensor->data = (int32_t *) calloc(LTC2448_SENSOR_CHANNEL_COUNT, sizeof(int32_t));

	sensor->update_all_sensor_data_fp = &ft2248_update_all_sensor_data;
	sensor->update_sensor_data_fp = &ft2248_update_sensor_data;
	sensor->get_sensor_data_fp = &ft2248_get_sensor_data;
	sensor->get_all_sensor_data_fp = &ft2248_get_all_sensor_data;
	sensor->set_sensor_data_fp = &ft2248_set_sensor_data;
	sensor->set_all_sensor_data_fp = &ft2248_set_all_sensor_data;
	
	return sensor;
}

void delete_ft2248(sensor_t * sensor){
	free(sensor->data);
	delete_sensor(sensor);
}

void ft2248_update_all_sensor_data(sensor_t *sensor){
	printf ("%s Entered\r\n", __func__);
	
}
void ft2248_update_sensor_data(sensor_t *sensor, uint8_t channel){
	printf ("%s Entered\r\n", __func__);
}
void ft2248_get_sensor_data(sensor_t * sensor, uint8_t channel, void * data){
	printf ("%s Entered\r\n", __func__);
}
void ft2248_get_all_sensor_data(sensor_t * sensor, void * data){
	printf ("%s Entered\r\n", __func__);
}
void ft2248_set_sensor_data(sensor_t * sensor, uint8_t channel, void * data){
	printf ("%s Entered\r\n", __func__);
}
void ft2248_set_all_sensor_data(sensor_t *sensor, void *data){
	printf ("%s Entered\r\n", __func__);
}