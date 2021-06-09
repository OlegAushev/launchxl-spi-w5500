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

#define W5500_MAX_DATA_SIZE 1024

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

	static uint8_t buff_[W5500_MAX_DATA_SIZE];

	static void ChipSelect();
	static void ChipDeselect();
	static void WriteByte(uint8_t data);
	static uint8_t ReadByte();
	static void WriteBuff(uint8_t* p_buff, uint16_t len);
	static void ReadBuff(uint8_t* p_buff, uint16_t len);

public:
	W5500_Controller(mcu::SpiModule module, wiz_NetInfo* netinfo, W5500_UdpSettings udp_settings);
	int32_t Send(uint8_t* buff, uint16_t len);
	template <typename T, uint32_t array_size>
	int32_t Send(const BasicArray<T, array_size>& data);
};

/**
 * @brief
 * @param
 * @return
 */
template <typename T, uint32_t array_size>
int32_t W5500_Controller::Send(const BasicArray<T, array_size>& data)
{
	if (sizeof(T) != 2)	/* Only arrays of 32-bit elements are supported now */
	{
		return SOCKERR_ARG;
	}

	if (sizeof(T)*array_size*2 > W5500_MAX_DATA_SIZE ) /* C28 byte is 16-bit long */
	{
		return SOCKERR_DATALEN;
	}

	for (size_t i = 0; i < data.size(); ++i)	/* split into 8-bit words */
	{
		uint16_t c28_byte[2];
		memcpy(&c28_byte, &data[i], 2);
		uint8_t byte[4];
		byte[3] = (c28_byte[1] >> 8) & 0x00FF;
		byte[2] = c28_byte[1] & 0x00FF;
		byte[1] = (c28_byte[0] >> 8) & 0x00FF;
		byte[0] = c28_byte[0] & 0x00FF;
		for (int j = 0; j < 4; ++j)
		{
			buff_[4*i+j] = byte[j];
		}
	}

	return Send((uint8_t*)buff_, data.size()*4);
}











#endif /* SRC_COMM_CONTROLLER_W5500_CONTROLLER_W5500_H_ */
