/*
 * nRF.c
 *
 * Created: 2022-05-28 15:47:41
 *  Author: Marek
 
 Na podstawie pracy msalamon :
 https://msalamon.pl/komunikacja-radiowa-z-uzyciem-modulow-nrf24l01-cz-1/
 
 */ 

#include "../GPIO/GPIO.h"
#include "../SPI/SPI.h"

#include "nRFDef.h"
#include "nRF.h"

#include <avr/io.h>
#include <util/delay.h>


static uint8_t addr_p0_backup[NRF24_ADDR_SIZE];

extern volatile uint8_t nrf24_rx_flag, nrf24_tx_flag, nrf24_mr_flag;

//
// BASIC READ/WRITE FUNCTIONS

static void nRF24_Delay(uint8_t Time)
{
	_delay_ms( 1 );
}

static void nRF24_SendSpi(uint8_t *Data, gpioPin_t nSS ,uint8_t Length)
{
	spiWrite(Data, nSS , Length);
}

static void nRF24_ReadSpi(uint8_t *Data, gpioPin_t nSS, uint8_t Length)
{
	spiRead(Data, nSS, Length);
}

//
// END OF BASIC READ/WRITE FUNCTIONS
//

static uint8_t nRF24_ReadRegister(uint8_t reg, gpioPin_t nSS)
{
	uint8_t result;

	reg = NRF24_CMD_R_REGISTER | reg;

	nRF24_SendSpi(&reg, nSS, 1);
	nRF24_ReadSpi(&result, nSS, 1);

	return result;
}

static void nRF24_ReadRegisters(uint8_t reg, uint8_t* ret, uint8_t len, gpioPin_t nSS)
{
	reg = NRF24_CMD_R_REGISTER | reg;

	clearPin(nSS);

	nRF24_SendSpi(&reg, nSS, 1);
	nRF24_ReadSpi(ret, nSS, len);

	setPin(nSS);
}

static void nRF24_WriteRegister(uint8_t reg, uint8_t val, gpioPin_t nSS)
{
	uint8_t tmp[2];

	tmp[0] = NRF24_CMD_W_REGISTER | reg;
	tmp[1] = val;
	
	nRF24_SendSpi(tmp, nSS, 2);
}

static void nRF24_WriteRegisters(uint8_t reg, uint8_t* val, uint8_t len, gpioPin_t nSS)
{
	reg = NRF24_CMD_W_REGISTER | reg;
	
	nRF24_SendSpi(&reg, nSS, 1);
	nRF24_SendSpi(val, nSS, len);
}

void nRF24_RX_Mode(gpioPin_t CE, gpioPin_t nSS)
{
	uint8_t config = nRF24_ReadConfig(nSS);
	// Restore pipe 0 adress after comeback from TX mode
	nRF24_SetRXAddress(0, addr_p0_backup, nSS);
	// PWR_UP bit set
	config |= (1<<NRF24_PWR_UP);
	// PRIM_RX bit set
	config |= (1<<NRF24_PRIM_RX);
	nRF24_WriteConfig(config, nSS);
	// Reset status
	nRF24_WriteStatus((1<<NRF24_RX_DR)|(1<<NRF24_TX_DS)|(1<<NRF24_MAX_RT), nSS);
	// Flush RX
	nRF24_FlushRX(nSS);
	// Flush TX
	nRF24_FlushTX(nSS);

	setPin(CE);
	nRF24_Delay(1);
}

void nRF24_TX_Mode(gpioPin_t CE, gpioPin_t nSS)
{
	clearPin(CE);

	uint8_t config = nRF24_ReadConfig(nSS);
	// PWR_UP bit set
	config |= (1<<NRF24_PWR_UP);
	// PRIM_RX bit low
	config &= ~(1<<NRF24_PRIM_RX);
	nRF24_WriteConfig(config, nSS);
	// Reset status
	nRF24_WriteStatus((1<<NRF24_RX_DR)|(1<<NRF24_TX_DS)|(1<<NRF24_MAX_RT), nSS);
	// Flush RX
	nRF24_FlushRX(nSS);
	// Flush TX
	nRF24_FlushTX(nSS);

	nRF24_Delay(1);
}



uint8_t nRF24_ReadConfig(gpioPin_t nSS)
{
	return (nRF24_ReadRegister(NRF24_CONFIG, nSS));
}

void nRF24_WriteConfig(uint8_t conf, gpioPin_t nSS)
{
	nRF24_WriteRegister(NRF24_CONFIG, conf, nSS);
}

void nRF24_SetPALevel(uint8_t lev, gpioPin_t nSS)
{
	uint8_t rf_setup = nRF24_ReadRegister(NRF24_RF_SETUP, nSS);
	rf_setup &= 0xF8; // Clear PWR bits
	rf_setup |= (lev<<1);
	nRF24_WriteRegister(NRF24_RF_SETUP, rf_setup, nSS);
}

void nRF24_SetDataRate(uint8_t dr, gpioPin_t nSS)
{
	uint8_t rf_setup = nRF24_ReadRegister(NRF24_RF_SETUP, nSS);
	rf_setup &= 0xD7; // Clear DR bits (1MBPS)
	if(dr == NRF24_RF_DR_250KBPS)
		rf_setup |= (1<<NRF24_RF_DR_LOW);
	else if(dr == NRF24_RF_DR_2MBPS)
		rf_setup |= (1<<NRF24_RF_DR_HIGH);
	nRF24_WriteRegister(NRF24_RF_SETUP, rf_setup, nSS);
}

uint8_t nRF24_ReadStatus(gpioPin_t nSS)
{
	return (nRF24_ReadRegister(NRF24_STATUS, nSS));
}

void nRF24_WriteStatus(uint8_t st, gpioPin_t nSS)
{
	nRF24_WriteRegister(NRF24_STATUS, st, nSS);
}

void nRF24_FlushRX(gpioPin_t nSS)
{
	uint8_t command = NRF24_CMD_FLUSH_RX;

	nRF24_SendSpi(&command, nSS, 1);
}

void nRF24_FlushTX(gpioPin_t nSS)
{
	uint8_t command = NRF24_CMD_FLUSH_TX;
	
	nRF24_SendSpi(&command, nSS, 1);
}

void nRF24_EnableCRC(uint8_t onoff, gpioPin_t nSS)
{
	uint8_t config = nRF24_ReadConfig(nSS);

	if(onoff)
		config |= (1<<NRF24_EN_CRC);
	else
		config &= ~(1<<NRF24_EN_CRC);
	nRF24_WriteConfig(config, nSS);
}

void nRF24_SetCRCLength(uint8_t crcl, gpioPin_t nSS)
{
	uint8_t config = nRF24_ReadConfig(nSS);
	if(crcl == NRF24_CRC_WIDTH_2B)
		config |= (1<<NRF24_CRCO);
	else
		config &= ~(1<<NRF24_CRCO);
	nRF24_WriteConfig(config, nSS);
}

void nRF24_SetRetries(uint8_t ard, uint8_t arc, gpioPin_t nSS)
{
	// ard * 250us, arc repeats
	nRF24_WriteRegister(NRF24_SETUP_RETR, (((ard & 0x0F)<<NRF24_ARD) | ((arc & 0x0F)<<NRF24_ARC)), nSS);
}

void nRF24_SetRFChannel(uint8_t channel, gpioPin_t nSS)
{
	nRF24_WriteRegister(NRF24_RF_CH, (channel & 0x7F), nSS);
}

void nRF24_SetPayloadSize(uint8_t pipe, uint8_t size, gpioPin_t nSS)
{
	if(pipe > 5)
		pipe = 5; // Block too high pipe number
	nRF24_WriteRegister(NRF24_RX_PW_P0 + pipe , (size & 0x3F), nSS);
}

void nRF24_EnablePipe(uint8_t pipe, uint8_t onoff, gpioPin_t nSS)
{
	if(pipe > 5)
		pipe = 5; // Block too high pipe number
	uint8_t enable_pipe = nRF24_ReadRegister(NRF24_EN_RXADDR, nSS);
	if(onoff == 1)
		enable_pipe |= (1<<pipe);
	else
		enable_pipe &= ~(1<<pipe);
	nRF24_WriteRegister(NRF24_EN_RXADDR, enable_pipe, nSS);
}

void nRF24_AutoACK(uint8_t pipe, uint8_t onoff, gpioPin_t nSS)
{
	if(pipe > 5)
		pipe = 5; // Block too high pipe number
	uint8_t enaa = nRF24_ReadRegister(NRF24_EN_AA, nSS);
	if(onoff == 1)
		enaa |= (1<<pipe);
	else
		enaa &= ~(1<<pipe);
	nRF24_WriteRegister(NRF24_EN_AA, enaa, nSS);
}

void nRF24_SetAddressWidth(uint8_t size, gpioPin_t nSS)
{
	if(size > 5)
		size = 5; // Maximum are 5 bytes
	if(size < 3)
		size = 3; // Minimum are 3 bytes
	nRF24_WriteRegister(NRF24_SETUP_AW, ((size-2) & 0x03), nSS);
}

void nRF24_SetRXAddress(uint8_t pipe, uint8_t* address, gpioPin_t nSS)
{
	// pipe 0 and pipe 1 are fully 40-bits storaged
	// pipe 2-5 is storaged only with last byte. Rest are as same as pipe 1
	// pipe 0 and 1 are LSByte first so they are needed to reverse address
	if((pipe == 0) || (pipe == 1))
	{
		uint8_t i;
		uint8_t address_rev[NRF24_ADDR_SIZE];
		for(i = 0; i<NRF24_ADDR_SIZE; i++)
			address_rev[NRF24_ADDR_SIZE - 1 - i] = address[i];
		nRF24_WriteRegisters(NRF24_RX_ADDR_P0 + pipe, address_rev, NRF24_ADDR_SIZE, nSS);
	}
	else
		nRF24_WriteRegister(NRF24_RX_ADDR_P0 + pipe, address[NRF24_ADDR_SIZE-1], nSS);
}

void nRF24_SetTXAddress(uint8_t* address, gpioPin_t nSS )
{
	// TX address is storaged similar to RX pipe 0 - LSByte first
	uint8_t i;
	uint8_t address_rev[NRF24_ADDR_SIZE];

	nRF24_ReadRegisters(NRF24_RX_ADDR_P0, address_rev, NRF24_ADDR_SIZE, nSS); // Backup P0 address
	for(i = 0; i<NRF24_ADDR_SIZE; i++)
		addr_p0_backup[NRF24_ADDR_SIZE - 1 - i] = address_rev[i]; //Reverse P0 address

	for(i = 0; i<NRF24_ADDR_SIZE; i++)
		address_rev[NRF24_ADDR_SIZE - 1 - i] = address[i];
	//make pipe 0 address backup;

	nRF24_WriteRegisters(NRF24_RX_ADDR_P0, address_rev, NRF24_ADDR_SIZE, nSS); // Pipe 0 must be same for auto ACk
	nRF24_WriteRegisters(NRF24_TX_ADDR, address_rev, NRF24_ADDR_SIZE, nSS);

}

void nRF24_ClearInterrupts(gpioPin_t nSS)
{
	uint8_t status = nRF24_ReadStatus(nSS);
	status |= (7<<4); // Clear bits 4, 5, 6.
	nRF24_WriteStatus(status, nSS);
}

void nRF24_EnableRXDataReadyIRQ(uint8_t onoff, gpioPin_t nSS)
{
	uint8_t config = nRF24_ReadConfig(nSS);

	if(!onoff)
		config |= (1<<NRF24_RX_DR);
	else
		config &= ~(1<<NRF24_RX_DR);

	nRF24_WriteConfig(config, nSS);
}

void nRF24_EnableTXDataSentIRQ(uint8_t onoff, gpioPin_t nSS)
{
	uint8_t config = nRF24_ReadConfig(nSS);

	if(!onoff)
		config |= (1<<NRF24_TX_DS);
	else
		config &= ~(1<<NRF24_TX_DS);

	nRF24_WriteConfig(config, nSS);
}

void nRF24_EnableMaxRetransmitIRQ(uint8_t onoff, gpioPin_t nSS)
{
	uint8_t config = nRF24_ReadConfig(nSS);

	if(!onoff)
		config |= (1<<NRF24_MAX_RT);
	else
		config &= ~(1<<NRF24_MAX_RT);

	nRF24_WriteConfig(config, nSS);
}

void nRF24_WriteTXPayload(uint8_t * data, gpioPin_t nSS)
{
	nRF24_WriteRegisters(NRF24_CMD_W_TX_PAYLOAD, data, NRF24_PAYLOAD_SIZE, nSS);
}

void nRF24_WaitTX(gpioPin_t CE, gpioPin_t nSS)
{
	uint8_t status;
	setPin(CE);
	nRF24_Delay(1);
	clearPin(CE);
	do
	{
		nRF24_Delay(1);
		status = nRF24_ReadStatus(nSS);
	}while(!((status & (1<<NRF24_MAX_RT)) || (status & (1<<NRF24_TX_DS))));

}

void nRF24_ReadRXPaylaod(uint8_t *data, gpioPin_t nSS)
{
	nRF24_ReadRegisters(NRF24_CMD_R_RX_PAYLOAD, data, NRF24_PAYLOAD_SIZE, nSS);
	nRF24_WriteRegister(NRF24_STATUS, (1<NRF24_RX_DR), nSS);
	if(nRF24_ReadStatus(nSS) & (1<<NRF24_TX_DS))
		nRF24_WriteRegister(NRF24_STATUS, (1<<NRF24_TX_DS), nSS);
}

void nRF24_SendPacket(uint8_t* data)
{

}

void nRF24_ReceivePacket(uint8_t* data)
{

}

uint8_t nRF24_RXAvailible(gpioPin_t nSS)
{
	uint8_t status = nRF24_ReadStatus(nSS);

	// RX FIFO Interrupt
	if ((status & (1 << 6)))
	{
		nrf24_rx_flag = 1;
		status |= (1<<6); // Interrupt flag clear
		nRF24_WriteStatus(status, nSS);
		return 1;
	}
	return 0;
}

//void nRF24_IRQ_Handler(void)
//{
	//uint8_t status = nRF24_ReadStatus();
//
	//// RX FIFO Interrupt
	//if ((status & (1 << NRF24_RX_DR)))
	//{
		//nrf24_rx_flag = 1;
		//status |= (1<<NRF24_RX_DR); // Interrupt flag clear
		//nRF24_WriteStatus(status);
	//}
	//// TX Data Sent interrupt
	//if ((status & (1 << NRF24_TX_DS)))
	//{
		//nrf24_tx_flag = 1;
		//status |= (1<<NRF24_TX_DS); // Interrupt flag clear
		//nRF24_WriteStatus(status);
	//}
	//// Max Retransmits interrupt
	//if ((status & (1 << NRF24_MAX_RT)))
	//{
		//nrf24_mr_flag = 1;
		//status |= (1<<NRF24_MAX_RT); // Interrupt flag clear
		//nRF24_WriteStatus(status);
	//}
//}

void nRF24_Init(gpioPin_t CE, gpioPin_t nSS)
{
	clearPin(CE);
	setPin(nSS);

	nRF24_Delay(5); // Wait for radio power up

	nRF24_SetPALevel(NRF24_PA_PWR_0dBM, nSS); // Radio power
	nRF24_SetDataRate(NRF24_RF_DR_250KBPS, nSS); // Data Rate
	nRF24_EnableCRC(1, nSS); // Enable CRC
	nRF24_SetCRCLength(NRF24_CRC_WIDTH_1B, nSS); // CRC Length 1 byte
	nRF24_SetRetries(0x04, 0x07, nSS); // 1000us, 7 times
	nRF24_WriteRegister(NRF24_DYNPD, 0, nSS); // Disable dynamic payloads for all pipes
	nRF24_SetRFChannel(10, nSS); // Set RF channel for transmission
	nRF24_SetPayloadSize(0, NRF24_PAYLOAD_SIZE, nSS); // Set 32 bytes payload for pipe 0
	nRF24_EnablePipe(0, 1, nSS); // Enable pipe 0
	nRF24_AutoACK(0, 1, nSS); // Enable auto ACK for pipe 0
	nRF24_SetAddressWidth(NRF24_ADDR_SIZE, nSS); // Set address size

	nRF24_Delay(20);

	//nRF24_EnableRXDataReadyIRQ(0);
	//nRF24_EnableTXDataSentIRQ(0);
	//nRF24_EnableMaxRetransmitIRQ(0);

	nRF24_Delay(20);

	nRF24_ClearInterrupts(nSS);
}
