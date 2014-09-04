#ifndef __ANNA_LOGGER_WIFI_SPI__
#define __ANNA_LOGGER_WIFI_SPI__


#include <stdint.h>
//#include <wlan.h>
typedef void (*gcSpiHandleRx)(void *p);
typedef void (*gcSpiHandleTx)(void);

extern uint8_t wlan_tx_buffer[];

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
} spi_information_t;

extern spi_information_t spi_information;

//uint8_t wlan_tx_buffer[];
//uint8_t wlan_rx_buffer[];
void spi_wifi_init(void);
/*
uint8_t spi_transfer_byte(uint8_t data_out);
void spi_transfer(void * data_out, uint16_t data_out_length, void * data_in, uint16_t data_in_length);
*/


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
