#ifndef __ANNA_LOGGER_WIFI_SPI__
#define __ANNA_LOGGER_WIFI_SPI__


#include <stdint.h>
//#include <wlan.h>
typedef void (*gcSpiHandleRx)(void *p);
typedef void (*gcSpiHandleTx)(void);

#include <asf.h>

extern xQueueHandle * WIFI_queue;

extern uint8_t wlan_tx_buffer[];


typedef struct _wifi_queue_type_t {
	bool interrupt;
	uint8_t * data;
} wifi_queue_type_t;

//Mandatory Functions Are:

/*
	SpiOpen
	SpiWrite
	SpiRead
	SpiClose
	SpiResumeSpi
	ReadWlanInterruptPin
	WlanInterruptEnable
	WlanInterruptDisable
	WriteWlanPin
*/


typedef struct
{
	gcSpiHandleRx  spi_rx_handler;

	uint32_t		spi_state;

	uint8_t			*rx_packet;
	uint16_t		rx_packet_length;

	uint8_t			*tx_packet;
	uint16_t		tx_packet_length;
	
	wifi_queue_type_t interrupt_semaphore;
} spi_information_t;

extern spi_information_t spi_information;

//uint8_t wlan_tx_buffer[];
//uint8_t wlan_rx_buffer[];
void spi_wifi_init(void);



extern void SpiClose(void);
extern void SpiOpen(gcSpiHandleRx pfRxHandler);
extern int16_t SpiWrite(uint8_t *data_out, uint16_t length);
extern int16_t SpiRead(uint8_t *data_in, uint16_t length);
extern void SpiPauseSpi(void);
extern void SpiResumeSpi(void);
extern int16_t ReadWlanInterruptPin(void);
extern void WlanInterruptEnable(void);
extern void WlanInterruptDisable(void);
extern void WriteWlanPin(uint8_t val);


void cc3k_int_poll(void);

int8_t * sendDrivePatch(uint32_t *length);
int8_t * sendBootLoaderPatch(uint32_t *length);
int8_t * sendWLFWPatch(uint32_t *length);

#endif
