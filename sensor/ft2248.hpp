

#ifndef __FT2234_H__
#define __FT2234_H__

#include "sensor.hpp"

#define LTC2448_SENSOR					0
#define LTC2448_SENSOR_CHANNEL_COUNT	16


//Rates
#define ADC_SAMPLE_RATE_3P52KHZ		= 0x02;
#define ADC_SAMPLE_RATE_1P76KHZ		= 0x04;
#define ADC_SAMPLE_RATE_880HZ		= 0x06;
#define ADC_SAMPLE_RATE_440HZ		= 0x08;
#define ADC_SAMPLE_RATE_220HZ		= 0x0A;
#define ADC_SAMPLE_RATE_110HZ		= 0x0C;
#define ADC_SAMPLE_RATE_55HZ		= 0x0E;
#define ADC_SAMPLE_RATE_27P5HZ		= 0x10;
#define ADC_SAMPLE_RATE_13P75HZ		= 0x12;
#define ADC_SAMPLE_RATE_6P875HZ		= 0x1D;

#define ADC_SAMPLE_RATE_7P04KHZ_T	= 0x03;
#define ADC_SAMPLE_RATE_3P52KHZ_T	= 0x05;
#define ADC_SAMPLE_RATE_1P76KHZ_T	= 0x07;
#define ADC_SAMPLE_RATE_880HZ_T		= 0x09;
#define ADC_SAMPLE_RATE_440HZ_T		= 0x0B;
#define ADC_SAMPLE_RATE_220HZ_T		= 0x0D;
#define ADC_SAMPLE_RATE_110HZ_T		= 0x0F;
#define ADC_SAMPLE_RATE_55HZ_T		= 0x11;
#define ADC_SAMPLE_RATE_27P5HZ_T	= 0x13;
#define ADC_SAMPLE_RATE_13P75HZ_T	= 0x1F;


class FT2248: Public SensorBase{
	private:
		int32_t adc_values[LTC2448_SENSOR_CHANNEL_COUNT];
		
	public:
	
	//Constructor/Destructor
	FT2248(uint8_t address);
	~FT2248();

};

#endif