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

struct W5500_UdpSettings
{
	uint8_t socket_rx;
	uint16_t port_rx;
	uint8_t socket_tx;
	uint16_t port_tx;
	uint8_t ip_tx[4];
};

class W5500_Controller
{
private:
	mcu::Spi spi_;
	static uint32_t spi_base_;
	static uint32_t cs_pin_;
	W5500_UdpSettings udp_;

	static void ChipSelect();
	static void ChipDeselect();
	static void WriteByte(uint8_t data);
	static uint8_t ReadByte();
	static void WriteBuff(uint8_t* p_buff, uint16_t len);
	static void ReadBuff(uint8_t* p_buff, uint16_t len);

public:
	W5500_Controller(mcu::SpiModule module, SPI_TransferProtocol protocol, SPI_Mode mode,
					uint32_t bitrate, uint16_t data_width, mcu::SpiTeMode te_mode,
					wiz_NetInfo* netinfo, W5500_UdpSettings udp_settings);

	int32_t Send(uint8_t* buff, uint16_t len);
};


#endif /* SRC_COMM_CONTROLLER_W5500_CONTROLLER_W5500_H_ */
