

#include <stdint.h>
#include <anna_logger_wifi_spi.h>

#include <asf.h>
#include <user_board.h>
#include "CC3000HostDriver/cc3000_common.h"


// The magic number that resides at the end of the TX/RX buffer (1 byte after the allocated size)
// for the purpose of detection of the overrun. The location of the memory where the magic number
// resides shall never be written. In case it is written - the overrun occured and either recevie function
// or send function will stuck forever.
#define CC3000_BUFFER_MAGIC_NUMBER (0xDE)

char spi_buffer[CC3000_RX_BUFFER_SIZE];
uint8_t wlan_tx_buffer[CC3000_TX_BUFFER_SIZE];

static volatile char ccspi_is_in_irq = 0;
static volatile char ccspi_int_enabled = 0;

extern struct spi_master_vec_module anna_wifi_master;

struct spi_master_vec_bufdesc spi_data_out[2];
struct spi_master_vec_bufdesc spi_data_in[2];

spi_information_t spi_information;

#define HI(value)                       (((value) & 0xFF00) >> 8)
#define LO(value)                       ((value) & 0x00FF)

#define SPI_STATE_POWERUP				(0)
#define SPI_STATE_INITIALIZED			(1)
#define SPI_STATE_IDLE					(2)
#define SPI_STATE_WRITE_IRQ				(3)
#define SPI_STATE_WRITE_FIRST_PORTION	(4)
#define SPI_STATE_WRITE_EOT				(5)
#define SPI_STATE_READ_IRQ				(6)
#define SPI_STATE_READ_FIRST_PORTION	(7)
#define SPI_STATE_READ_EOT				(8)

void spi_wifi_init(void){
	//Initialization is done in the init.c
	spi_master_vec_enable(&anna_wifi_master);
	spi_data_out[1].data = NULL;
	spi_data_out[1].length = 0;
	
	spi_data_in[1].data = NULL;
	spi_data_in[1].length = 0;
	port_pin_set_output_level(ANNA_WIFI_CS_N, HIGH);
}
uint8_t spi_transfer_byte(uint8_t data_out){
	uint8_t data_in = 0;
	spi_data_out[0].data = &data_out;
	spi_data_out[0].length = 1;
	
	spi_data_in[0].data = &data_in;
	spi_data_in[0].length = 1;
	
	spi_master_vec_lock(&anna_wifi_master);
	port_pin_set_output_level(ANNA_WIFI_CS_N, LOW);
	
	spi_master_vec_transceive_buffer_wait(&anna_wifi_master, spi_data_out, spi_data_in);
	
	port_pin_set_output_level(ANNA_WIFI_CS_N, HIGH);
	spi_master_vec_unlock(&anna_wifi_master);
	return data_in;
}
void spi_transfer(void * data_out, uint16_t data_out_length, void * data_in, uint16_t data_in_length){
	spi_data_out[0].data = &data_out;
	spi_data_out[0].length = data_out_length;
	
	spi_data_in[0].data = &data_in;
	spi_data_in[0].length = data_in_length;
	
	spi_master_vec_lock(&anna_wifi_master);
	port_pin_set_output_level(ANNA_WIFI_CS_N, LOW);
	
	spi_master_vec_transceive_buffer_wait(&anna_wifi_master, spi_data_out, spi_data_in);
	
	port_pin_set_output_level(ANNA_WIFI_CS_N, HIGH);
	spi_master_vec_unlock(&anna_wifi_master);
}

static void wifi_interrupt_callback(void){

	
}

void SpiClose(void){
	if (spi_information.rx_packet){
		spi_information.rx_packet = 0;
	}
	
	spi_master_vec_disable(&anna_wifi_master);
	WlanInterruptDisable();
	port_pin_set_output_level(ANNA_WIFI_CS_N, HIGH);
}
void SpiOpen(gcSpiHandleRx pfRxHandler){

	//Structure for external interrupt channel configuration
	struct extint_chan_conf eic_conf;
	spi_information.spi_state = SPI_STATE_POWERUP;
	
	//Initialization is done in the init.c
	spi_master_vec_enable(&anna_wifi_master);
	spi_data_out[1].data = NULL;
	spi_data_out[1].length = 0;
	
	spi_data_in[1].data = NULL;
	spi_data_in[1].length = 0;
	port_pin_set_output_level(ANNA_WIFI_CS_N, HIGH);
	
	spi_information.spi_rx_handler		= pfRxHandler;
	spi_information.tx_packet_length	= 0;
	spi_information.tx_packet			= NULL;
	spi_information.rx_packet			= spi_buffer;
	spi_information.rx_packet_length	= 0;
	
	spi_buffer[CC3000_RX_BUFFER_SIZE - 1]		=	CC3000_BUFFER_MAGIC_NUMBER;
	wlan_tx_buffer[CC3000_TX_BUFFER_SIZE - 1]	=	CC3000_BUFFER_MAGIC_NUMBER;
	
	tSLInformation.WlanInterruptEnable();
		
	
	//Setting up the external interrupt	
	extint_chan_get_config_defaults(&eic_conf);
	eic_conf.gpio_pin = WIFI_IRQ_PIN;
	eic_conf.gpio_pin_mux = WIFI_IRQ_MUX;
	eic_conf.gpio_pin_pull = EXTINT_PULL_UP;
	eic_conf.detection_criteria = EXTINT_DETECT_RISING;
	
	//Setup the external interrupt
	extint_chan_set_config(     WIFI_IRQ_CHANNEL,
	                            &eic_conf);
	
	//Register and Enable the callback function
	extint_register_callback(   wifi_interrupt_callback,
							    WIFI_IRQ_CHANNEL,
							    EXTINT_CALLBACK_TYPE_DETECT);
//	WlanInterruptEnable();
}
int16_t SpiWrite(uint8_t *data_out, uint16_t length){
	uint8_t pad = 0;
	
	if (!(length & 0x0001)){
		pad++;	
	}
	
	data_out[0] = WRITE;
	data_out[1] = HI(length + pad);
	data_out[2] = LO(length + pad);
	data_out[3]	= 0;
	data_out[4] = 0;
	
	length += (SPI_HEADER_SIZE + pad);
	
	//The magic number that resides at the end of the TX/RX buffer (1 byte after the allocation size)
	//	for the purpose of overrun detection.
	//	occurred we will be stuck here forever
	
	if (wlan_tx_buffer[CC33000_TX_BUFFER_SIZE - 1] != CC3000_BUFFER_MAGIC_NUMBER){
		//Error, no magic number found!
		while (1);
	}
	
	//Setup the CS_N
	port_pin_set_output_level(ANNA_WIFI_CS_N, LOW);
	
	//First Write
	vTaskDelay(1 / portTICK_RATE_MS);
	spi_transfer(data_out, 4);
	vTaskDelay(1 / portTICK_RATE_MS);
	spi_transfer(&data_out[4], length - 4);
	
	port_pin_set_output_level(ANNA_WIFI_CS_N, HIGH);
	
	spi_transfer(data_out, length, NULL, 0);
	return 0;
}
int16_t SpiRead(uint8_t *data_in, uint16_t length){
	return 0;
}
void SpiPauseSpi(void){
	WlanInterruptDisable();
}
void SpiResumeSpi(void){
	WlanInterruptEnable();
}
int16_t ReadWlanInterruptPin(void){
	return 0;
}
void WlanInterruptEnable(void){
	//Enable the callback
	extint_chan_enable_callback(WIFI_IRQ_CHANNEL,
	EXTINT_CALLBACK_TYPE_DETECT);	
}
void WlanInterruptDisable(void){
	//Disable the callback
	extint_chan_disable_callback(WIFI_IRQ_CHANNEL,
	EXTINT_CALLBACK_DETECT);
}
void WriteWlanPin(uint8_t val){
	//port_pin_set_output_level()
	
}