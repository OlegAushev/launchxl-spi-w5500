/*
 * controller_w5500.cpp
 *
 *  Created on: 9 апр. 2021 г.
 *      Author: Oleg Aushev
 */

#include "controller_w5500.h"

uint32_t SpiW5500::spi_base_;
uint32_t SpiW5500::cs_pin_;
void reg_wizchip_cs_cbfunc(void(*cs_sel)(void), void(*cs_desel)(void));
void reg_wizchip_spi_cbfunc(uint8_t (*spi_rb)(void), void (*spi_wb)(uint8_t wb));
void reg_wizchip_spiburst_cbfunc(void (*spi_rb)(uint8_t* pBuf, uint16_t len), void (*spi_wb)(uint8_t* pBuf, uint16_t len));

wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc, 0xab, 0xcd, 0xff},
                            .ip = {192, 168, 70, 250},
                            .sn = {255, 255, 255, 0},
                            .gw = {192, 168, 70, 1},
                            .dns = {0, 0, 0, 0},
                            .dhcp = NETINFO_STATIC };

/**
 * @brief Configures W5500
 * @param module SPI module
 * @return None(constructor)
 */
SpiW5500::SpiW5500(mcu::SpiModule module, SPI_TransferProtocol protocol, SPI_Mode mode,
								uint32_t bitrate, uint16_t data_width, mcu::SpiTeMode te_mode)
	: Spi(module, protocol, mode, bitrate, data_width, te_mode)
{
	SpiW5500::spi_base_ = this->GetBase();
	SpiW5500::cs_pin_ = this->GetTePin();

	reg_wizchip_cs_cbfunc(SpiW5500::ChipSelect, SpiW5500::ChipDeselect);
	reg_wizchip_spi_cbfunc(SpiW5500::ReadByte, SpiW5500::WriteByte);
	reg_wizchip_spiburst_cbfunc(SpiW5500::ReadBuff, SpiW5500::WriteBuff);

	uint8_t rx_tx_buff_sizes[] = {2, 2, 2, 2, 2, 2, 2, 2};
	wizchip_init(rx_tx_buff_sizes, rx_tx_buff_sizes);
	wizchip_setnetinfo(&gWIZNETINFO);
}

/**
 * @brief
 * @param
 * @return
 */
void SpiW5500::ChipSelect()
{
	GPIO_writePin(SpiW5500::cs_pin_, 0);
}

/**
 * @brief
 * @param
 * @return
 */
void SpiW5500::ChipDeselect()
{
	GPIO_writePin(SpiW5500::cs_pin_, 1);
}

/**
 * @brief
 * @param
 * @return
 */
void SpiW5500::WriteByte(uint8_t data)
{

	SPI_writeDataBlockingFIFO(SpiW5500::spi_base_, data << 8);
	SPI_readDataBlockingFIFO(SpiW5500::spi_base_);	// clear FIFO
}

/**
 * @brief
 * @param
 * @return
 */
uint8_t SpiW5500::ReadByte()
{
	SPI_writeDataBlockingFIFO(SpiW5500::spi_base_, 0xAAAA);	// send dummy data on TX line and generate clock on CLK line
	return SPI_readDataBlockingFIFO(SpiW5500::spi_base_) & 0x00FF;
}

/**
 * @brief
 * @param
 * @return
 */
void SpiW5500::WriteBuff(uint8_t* p_buff, uint16_t len)
{
	for (size_t i = 0; i < len; ++i)
	{
		SPI_writeDataBlockingFIFO(SpiW5500::spi_base_, p_buff[i] << 8);
		SPI_readDataBlockingFIFO(SpiW5500::spi_base_);	// clear FIFO
	}
}

/**
 * @brief
 * @param
 * @return
 */
void SpiW5500::ReadBuff(uint8_t* p_buff, uint16_t len)
{
	if (len <= 16)
	{
		for (size_t i = 0; i < len; ++i)
		{
			SPI_writeDataBlockingFIFO(SpiW5500::spi_base_, 0xAAAA);	// send dummy data on TX line and generate clock on CLK line
		}
		for (size_t i = 0; i < len; ++i)
		{
			p_buff[i] = SPI_readDataBlockingFIFO(SpiW5500::spi_base_) & 0x00FF;
		}
	}
	else
	{
		for (size_t i = 0; i < len; ++i)
		{
			p_buff[i] = SpiW5500::ReadByte();
		}
	}

}


