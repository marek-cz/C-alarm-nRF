/*
 * nRF.h
 *
 * Created: 2022-05-28 15:52:21
 *  Author: Marek
 
  Na podstawie pracy msalamon :
  https://msalamon.pl/komunikacja-radiowa-z-uzyciem-modulow-nrf24l01-cz-1/
 
 */ 


#ifndef NRF_H_
#define NRF_H_



void nRF24_Init(gpioPin_t CE, gpioPin_t nSS);

//
// READ/WRITE REGISTERS
//
uint8_t nRF24_ReadConfig(gpioPin_t nSS);
void nRF24_WriteConfig(uint8_t conf, gpioPin_t nSS);
uint8_t nRF24_ReadStatus(gpioPin_t nSS);
void nRF24_WriteStatus(uint8_t st, gpioPin_t nSS);

//
// SWITCHING BETWEEN RX AND TX
//
void nRF24_RX_Mode(gpioPin_t CE, gpioPin_t nSS);
void nRF24_TX_Mode(gpioPin_t CE, gpioPin_t nSS);

//
// RADIO SETTINGS
//
void nRF24_SetPALevel(uint8_t lev, gpioPin_t nSS);
void nRF24_SetDataRate(uint8_t dr, gpioPin_t nSS);
void nRF24_EnableCRC(uint8_t onoff, gpioPin_t nSS);
void nRF24_SetCRCLength(uint8_t crcl, gpioPin_t nSS);
void nRF24_SetRetries(uint8_t ard, uint8_t arc, gpioPin_t nSS);
void nRF24_SetRFChannel(uint8_t channel, gpioPin_t nSS);
void nRF24_SetPayloadSize(uint8_t pipe, uint8_t size, gpioPin_t nSS);
void nRF24_EnablePipe(uint8_t pipe, uint8_t onoff, gpioPin_t nSS);
void nRF24_AutoACK(uint8_t pipe, uint8_t onoff, gpioPin_t nSS);
void nRF24_SetRXAddress(uint8_t pipe, uint8_t* address, gpioPin_t nSS); // Remember to define RX address before TX
void nRF24_SetTXAddress(uint8_t* address, gpioPin_t nSS);
void nRF24_SetAddressWidth(uint8_t size, gpioPin_t nSS);

//
// INTERRUPT CONTROL
//
void nRF24_ClearInterrupts(gpioPin_t nSS);
void nRF24_EnableRXDataReadyIRQ(uint8_t onoff, gpioPin_t nSS);
void nRF24_EnableTXDataSentIRQ(uint8_t onoff, gpioPin_t nSS);
void nRF24_EnableMaxRetransmitIRQ(uint8_t onoff, gpioPin_t nSS);

//
// PUSH/PULL DATA TO PAYLOAD
//
void nRF24_WriteTXPayload(uint8_t* data, gpioPin_t nSS);
void nRF24_WaitTX( gpioPin_t CE, gpioPin_t nSS);
void nRF24_ReadRXPaylaod(uint8_t* data, gpioPin_t nSS);

//
// TRANSMITTING DATA
//
void nRF24_SendPacket(uint8_t* data);
void nRF24_ReceivePacket(uint8_t* data);

//
// FLUSHING FIFOs
//
void nRF24_FlushRX(gpioPin_t nSS);
void nRF24_FlushTX(gpioPin_t nSS);

//
// IRQ HANDLER
//
void nRF24_IRQ_Handler(void);

//
// POLLING METHOD
//
uint8_t nRF24_RXAvailible( gpioPin_t nSS);


#endif /* NRF_H_ */