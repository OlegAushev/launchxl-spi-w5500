/*
 * controller_w5500.h
 *
 *  Created on: 9 апр. 2021 г.
 *      Author: Oleg Aushev
 */

#ifndef SRC_COMM_CONTROLLER_W5500_CONTROLLER_W5500_H_
#define SRC_COMM_CONTROLLER_W5500_CONTROLLER_W5500_H_

#include <mcu/spi/spi_mcu.h>

class SpiW5500 : public mcu::Spi
{
private:
	static uint32_t spi_base_;

public:
	SpiW5500(mcu::SpiModule module, SPI_TransferProtocol protocol, SPI_Mode mode,
					uint32_t bitrate, uint16_t data_width);

	static void WriteByte(uint8_t data);
	static uint8_t ReadByte();

};



#endif /* SRC_COMM_CONTROLLER_W5500_CONTROLLER_W5500_H_ */
