/*
 * controller_w5500.cpp
 *
 *  Created on: 9 апр. 2021 г.
 *      Author: Oleg Aushev
 */

#include "controller_w5500.h"

uint32_t SpiW5500::spi_base_;
void reg_wizchip_cs_cbfunc(void(*cs_sel)(void), void(*cs_desel)(void));
void reg_wizchip_spi_cbfunc(uint8_t (*spi_rb)(void), void (*spi_wb)(uint8_t wb));
void reg_wizchip_spiburst_cbfunc(void (*spi_rb)(uint8_t* pBuf, uint16_t len), void (*spi_wb)(uint8_t* pBuf, uint16_t len));


/**
 * @brief Configures W5500
 * @param module SPI module
 * @return None(constructor)
 */
SpiW5500::SpiW5500(mcu::SpiModule module, SPI_TransferProtocol protocol, SPI_Mode mode,
								uint32_t bitrate, uint16_t data_width)
	: Spi(module, protocol, mode, bitrate, data_width)
{
	SpiW5500::spi_base_ = this->GetBase();

	reg_wizchip_spi_cbfunc(SpiW5500::ReadByte, SpiW5500::WriteByte);
	reg_wizchip_spiburst_cbfunc(SpiW5500::ReadBuff, SpiW5500::WriteBuff);
}

/**
 * @brief
 * @param
 * @return
 */
void SpiW5500::WriteByte(uint8_t data)
{
	SPI_writeDataBlockingNonFIFO(SpiW5500::spi_base_, data << 8);
}

/**
 * @brief
 * @param
 * @return
 */
uint8_t SpiW5500::ReadByte()
{
	return SPI_readDataBlockingNonFIFO(SpiW5500::spi_base_);
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
		SPI_writeDataBlockingNonFIFO(SpiW5500::spi_base_, p_buff[i] << 8);
	}
}

/**
 * @brief
 * @param
 * @return
 */
void SpiW5500::ReadBuff(uint8_t* p_buff, uint16_t len)
{
	for (size_t i = 0; i < len; ++i)
	{
		p_buff[i] = SPI_readDataBlockingNonFIFO(SpiW5500::spi_base_);
	}
}


