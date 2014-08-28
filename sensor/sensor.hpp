/*
 * sensor.h
 *
 * Created: 8/28/2014 12:09:39 PM
 *  Author: cospan
 */ 


#ifndef SENSOR_H_
#define SENSOR_H_

#include <stdint.h>

//ADC Constants

//Address


//Daughter board Name Defines
//#define THERMAL_SENSOR					1
//#define POWER_SUPPLY_SENSOR				2

//#define LTC2448_SENSOR_CHANNEL_COUNT	16
//#define THERMAL_SENSOR_CHANNEL_COUNT	16
//#define POWER_SUPPLY_CHANNEL_COUNT		16


//SENSOR Value Structure


class SensorBase (){
	private:
		uint8_t type;
		uint8_t channel_count;
		uint8_t addres;
		void * data;
	public:
		//Constructor/Destructor
		SensorBase(uint8_t type, uint8_t channel_count, uint8_t address);
		~SensorBase();
		
		uint32_t get_data(uint8_t channel);
		uint8_t get_channel_count();
		uint8_t get_type();
		uint8_t get_address();
};


#endif /* LTC2448_H_ */