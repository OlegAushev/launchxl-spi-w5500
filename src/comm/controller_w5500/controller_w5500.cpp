/*
 * controller_w5500.cpp
 *
 *  Created on: 9 апр. 2021 г.
 *      Author: Oleg Aushev
 */

#include "controller_w5500.h"

uint32_t SpiW5500::spi_base_;

void wizchip_spi_writebyte(uint8_t wb)
{
	SpiW5500::WriteByte(wb);
}

uint8_t wizchip_spi_readbyte()
{
	return SpiW5500::ReadByte();
}


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
}

/**
 * @brief
 * @param
 * @return
 */
void SpiW5500::WriteByte(uint8_t data)
{
	SPI_writeDataNonBlocking(SpiW5500::spi_base_, data);
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




