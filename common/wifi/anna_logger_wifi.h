#ifndef __CONF_SPI_WIFI__
#define __CONF_SPI_WIFI__

void spi_wifi_init(void);
uint8_t spi_transfer_byte(uint8_t data_out);
void spi_transfer(void * data_out, uint16_t data_out_length, void * data_in, uint16_t data_in_length);

//#define CC3000_TX_BUFFER_SIZE 256
//#define CC3000_RX_BUFFER_SIZE 256

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
typedef void (*gcSpiHandleRx)(void *p);
typedef void (*gcSpiHandleTx)(void);

typedef struct
{
	gcSpiHandleRx  SPIRxHandler;

	uint16_t usTxPacketLength;
	uint16_t usRxPacketLength;
	uint32_t  ulSpiState;
	uint8_t *pTxPacket;
	uint8_t *pRxPacket;

} tSpiInformation;

tSpiInformation sSpiInformation;

//uint8_t wlan_tx_buffer[];
//uint8_t wlan_rx_buffer[];

void SpiClose(void);
void SpiOpen(gcSpiHandleRx pfRxHandler);
int16_t SpiWrite(uint8_t *data_out, uint16_t length);
int16_t SpiRead(uint8_t *data_in, uint16_t length);
void SpiResumeSpi(void);
int16_t ReadWlanInterruptPin(void);
void WlanInterruptEnable(void);
void WlanInterruptDisable(void);
void WriteWlanPin(uint8_t val);







#endif