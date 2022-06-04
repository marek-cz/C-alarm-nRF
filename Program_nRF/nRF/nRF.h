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

void nRF24_Init(gpioPin_t nSS_pin, gpioPin_t CE_pin);

//
// READ/WRITE REGISTERS
//
uint8_t nRF24_ReadConfig(void);
void nRF24_WriteConfig(uint8_t conf);
uint8_t nRF24_ReadStatus();
void nRF24_WriteStatus(uint8_t st);

//
// SWITCHING BETWEEN RX AND TX
//
void nRF24_RX_Mode(void);
void nRF24_TX_Mode(void);

//
// RADIO SETTINGS
//
void nRF24_SetPALevel(uint8_t lev);
void nRF24_SetDataRate(uint8_t dr);
void nRF24_EnableCRC(uint8_t onoff);
void nRF24_SetCRCLength(uint8_t crcl);
void nRF24_SetRetries(uint8_t ard, uint8_t arc);
void nRF24_SetRFChannel(uint8_t channel);
void nRF24_SetPayloadSize(uint8_t pipe, uint8_t size);
void nRF24_EnablePipe(uint8_t pipe, uint8_t onoff);
void nRF24_AutoACK(uint8_t pipe, uint8_t onoff);
void nRF24_SetRXAddress(uint8_t pipe, uint8_t* address); // Remember to define RX address before TX
void nRF24_SetTXAddress(uint8_t* address);
void nRF24_SetAddressWidth(uint8_t size);

//
// INTERRUPT CONTROL
//
void nRF24_ClearInterrupts(void);
void nRF24_EnableRXDataReadyIRQ(uint8_t onoff);
void nRF24_EnableTXDataSentIRQ(uint8_t onoff);
void nRF24_EnableMaxRetransmitIRQ(uint8_t onoff);

//
// PUSH/PULL DATA TO PAYLOAD
//
void nRF24_WriteTXPayload(uint8_t* data);
void nRF24_WaitTX();
void nRF24_ReadRXPaylaod(uint8_t* data);

//
// TRANSMITTING DATA
//
void nRF24_SendPacket(uint8_t* data);
void nRF24_ReceivePacket(uint8_t* data);

//
// FLUSHING FIFOs
//
void nRF24_FlushRX(void);
void nRF24_FlushTX(void);

//
// IRQ HANDLER
//
void nRF24_IRQ_Handler(void);

//
// POLLING METHOD
//
uint8_t nRF24_RXAvailible(void);

#endif /* NRF_H_ */