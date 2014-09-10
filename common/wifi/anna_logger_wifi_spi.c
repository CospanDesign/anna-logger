

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

#define READ                            (3)
#define WRITE                           (1)
#define HI(value)                       (((value) & 0xFF00) >> 8)
#define LO(value)                       ((value) & 0x00FF)
#define HEADERS_SIZE_EVNT               (SPI_HEADER_SIZE + 5)
#define SPI_HEADER_SIZE                 (5)

#define SPI_STATE_POWERUP				(0)
#define SPI_STATE_IDLE					(2)
#define SPI_STATE_WAIT_RESPONSE			(3)
#define SPI_STATE_DETECT_IRQ			(4)


bool IN_IRQ;
bool IRQ_ENABLE;

void spi_wifi_init(void){
	IN_IRQ = false;
	IRQ_ENABLE = false;
	//Initialization is done in the init.c
	//spi_master_vec_enable(&anna_wifi_master);
	spi_data_out[1].data = NULL;
	spi_data_out[1].length = 0;
	
	spi_data_in[1].data = NULL;
	spi_data_in[1].length = 0;
	port_pin_set_output_level(ANNA_WIFI_CS_N, HIGH);
}

static void wifi_interrupt_callback(void){
	IN_IRQ = true;
	
	switch (spi_information.spi_state){
		case (SPI_STATE_POWERUP):
			//First IRQ went low, we can start our initial transaction
			spi_information.spi_state = SPI_STATE_DETECT_IRQ;
			break;
		case (SPI_STATE_IDLE):
			//We are either waiting to start a transaction, or the device has sent a request to start a transaction
			if (port_pin_get_input_level(ANNA_WIFI_CS_N)){
				xQueueSendToFrontFromISR(WIFI_queue, &spi_information.interrupt_semaphore, pdFALSE); 
			}
			spi_information.spi_state = SPI_STATE_DETECT_IRQ;
			
			break;
		case (SPI_STATE_WAIT_RESPONSE):
			xQueueSendToFrontFromISR(WIFI_queue, &spi_information.interrupt_semaphore, pdFALSE); 		
			spi_information.spi_state = SPI_STATE_DETECT_IRQ;
			break;
		case (SPI_STATE_DETECT_IRQ):
			spi_information.spi_state = SPI_STATE_IDLE;
			break;
		default:
			break;
	}
	
	IN_IRQ = false;
	return;
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
	spi_information.rx_packet			= (uint8_t *)spi_buffer;
	spi_information.rx_packet_length	= 0;
	spi_information.interrupt_semaphore.interrupt	=	true;
	
	spi_buffer[CC3000_RX_BUFFER_SIZE - 1]		=	CC3000_BUFFER_MAGIC_NUMBER;
	wlan_tx_buffer[CC3000_TX_BUFFER_SIZE - 1]	=	CC3000_BUFFER_MAGIC_NUMBER;
	

		
	
	//Setting up the external interrupt	
	extint_chan_get_config_defaults(&eic_conf);
	eic_conf.gpio_pin = WIFI_IRQ_PIN;
	eic_conf.gpio_pin_mux = WIFI_IRQ_MUX;
	eic_conf.gpio_pin_pull = EXTINT_PULL_UP;
	eic_conf.detection_criteria = EXTINT_DETECT_FALLING;
	
	//Setup the external interrupt
	extint_chan_set_config(     WIFI_IRQ_CHANNEL,
	                            &eic_conf);
	
	//Register and Enable the callback function
	extint_register_callback(   wifi_interrupt_callback,
							    WIFI_IRQ_CHANNEL,
							    EXTINT_CALLBACK_TYPE_DETECT);
	tSLInformation.WlanInterruptEnable();								
//	WlanInterruptEnable();
}
int16_t SpiWrite(uint8_t *data_out, uint16_t length){
	uint8_t pad = 0;
	
	if (!(length & 0x0001)){
		pad++;	
	}
	
	//First 5 bytes are a header
	data_out[0] = WRITE;
	data_out[1] = HI(length + pad);
	data_out[2] = LO(length + pad);
	data_out[3]	= 0;
	data_out[4] = 0;
	
	length += pad;
	while (spi_information.spi_state == SPI_STATE_POWERUP){
		//Wait here until the interrupt line has gone low indicating the device is ready for the first transaction
	}
	
	//SPI is ours! (Don't need it we are the only task interfacing with this device
	port_pin_set_output_level(ANNA_WIFI_CS_N, LOW);
	while (spi_information.spi_state != SPI_STATE_DETECT_IRQ){
		//Wait here until the interrupt line has gone low indicating the device is ready for a transaction
	}
	WlanInterruptDisable();
	vTaskDelay(0.5 / portTICK_RATE_MS);
	spi_data_out[0].data = data_out;
	spi_data_out[0].length = SPI_HEADER_SIZE;
	spi_master_vec_transceive_buffer_wait(&anna_wifi_master, spi_data_out, NULL);
	vTaskDelay(0.5 / portTICK_RATE_MS);
	spi_data_out[0].data = &data_out[SPI_HEADER_SIZE];
	spi_data_out[0].length = length;
	spi_master_vec_transceive_buffer_wait(&anna_wifi_master, spi_data_out, NULL);
	
	port_pin_set_output_level(ANNA_WIFI_CS_N, HIGH);
	spi_information.spi_state = SPI_STATE_IDLE;
	WlanInterruptEnable();
	return 0;
}

int16_t SpiRead(uint8_t *data_in, uint16_t length){
	
	//First 5 bytes are a header
	uint8_t data_out[5] = { READ, 0, 0, 0, 0 };

	//This should already be low
	while (spi_information.spi_state != SPI_STATE_DETECT_IRQ){
		vTaskDelay(1 / portTICK_RATE_MS);
	}	
	port_pin_set_output_level(ANNA_WIFI_CS_N, LOW);
	vTaskDelay(1 / portTICK_RATE_MS);
	spi_data_out[0].data = data_out;
	spi_data_out[0].length = SPI_HEADER_SIZE;
	
	spi_data_in[0].data = spi_buffer;
	spi_data_in[0].length = SPI_HEADER_SIZE;
	
	spi_master_vec_transceive_buffer_wait(&anna_wifi_master, spi_data_out, spi_data_in);
	spi_information.rx_packet_length =  (spi_buffer[3] << 8) |  spi_buffer[4];
	vTaskDelay(1 / portTICK_RATE_MS);
	//Read the rest of the data
	//spi_data_in[0].data = &spi_buffer[SPI_HEADER_SIZE];
	spi_data_in[0].data = spi_buffer;
	spi_data_in[0].length = spi_information.rx_packet_length;
	
	spi_master_vec_transceive_buffer_wait(&anna_wifi_master, NULL, spi_data_in);
	
	port_pin_set_output_level(ANNA_WIFI_CS_N, HIGH);
	spi_information.spi_state = SPI_STATE_IDLE;

	return 0;
}
void SpiPauseSpi(void){
	IRQ_ENABLE = false;
	WlanInterruptDisable();
}
void SpiResumeSpi(void){
	if (!IRQ_ENABLE){
		WlanInterruptEnable();
		IRQ_ENABLE = true;
	}
	
}

int16_t ReadWlanInterruptPin(void){
	return port_pin_get_input_level(ANNA_WIFI_IRQ_PIN);
}
void WlanInterruptEnable(void){
	//Enable the callback
	extint_chan_enable_callback(WIFI_IRQ_CHANNEL, EXTINT_CALLBACK_TYPE_DETECT);	
	IRQ_ENABLE = true;
}
void WlanInterruptDisable(void){
	//Disable the callback
	extint_chan_disable_callback(WIFI_IRQ_CHANNEL,
	EXTINT_CALLBACK_TYPE_DETECT);
}
void WriteWlanPin(uint8_t val){
	port_pin_set_output_level(ANNA_WIFI_EN, val);
}

int8_t * sendDrivePatch(uint32_t *length){
	*length = 0;
	return NULL;
}
int8_t * sendBootLoaderPatch(uint32_t *length){
	*length = 0;
	return NULL;
}
int8_t * sendWLFWPatch(uint32_t *length){
	*length = 0;
	return NULL;	
}

void cc3k_int_poll(void){
	if ((port_pin_get_input_level(WIFI_IRQ_PIN) == LOW) && (!IRQ_ENABLE)){
		wifi_interrupt_callback();
	}
}