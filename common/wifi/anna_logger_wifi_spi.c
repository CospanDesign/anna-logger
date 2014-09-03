

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

//uint8_t extern wlan_tx_buffer[CC3000_TX_BUFFER_SIZE];
//uint8_t extern wlan_rx_buffer[CC3000_RX_BUFFER_SIZE];

struct spi_master_vec_bufdesc spi_data_out[2];
struct spi_master_vec_bufdesc spi_data_in[2];

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


void SpiClose(void){
	
}
void SpiOpen(gcSpiHandleRx pfRxHandler){
	
}
int16_t SpiWrite(uint8_t *data_out, uint16_t length){
	return 0;
}
int16_t SpiRead(uint8_t *data_in, uint16_t length){
	return 0;
}
void SpiResumeSpi(void){
	
}
int16_t ReadWlanInterruptPin(void){
	return 0;
}
void WlanInterruptEnable(void){
	
}
void WlanInterruptDisable(void){
	
}
void WriteWlanPin(uint8_t val){
	
}