/*
 * controller_w5500.h
 *
 *  Created on: 9 апр. 2021 г.
 *      Author: Oleg Aushev
 */

#ifndef SRC_COMM_CONTROLLER_W5500_CONTROLLER_W5500_H_
#define SRC_COMM_CONTROLLER_W5500_CONTROLLER_W5500_H_

#include <mcu/spi/spi_mcu.h>
#include <ioLibrary_Driver/Ethernet/socket.h>


class SpiW5500 : public mcu::Spi
{
private:
	static uint32_t spi_base_;

public:
	SpiW5500(mcu::SpiModule module, SPI_TransferProtocol protocol, SPI_Mode mode,
					uint32_t bitrate, uint16_t data_width);

	static void WriteByte(uint8_t data);
	static uint8_t ReadByte();
	static void WriteBuff(uint8_t* p_buff, uint16_t len);
	static void ReadBuff(uint8_t* p_buff, uint16_t len);
};


#endif /* SRC_COMM_CONTROLLER_W5500_CONTROLLER_W5500_H_ */
