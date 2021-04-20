/*
 * controller_w5500.cpp
 *
 *  Created on: 9 апр. 2021 г.
 *      Author: Oleg Aushev
 */

#include <comm/w5500_controller/w5500_controller.h>

uint32_t W5500_Controller::spi_base_;
uint32_t W5500_Controller::cs_pin_;
void reg_wizchip_cs_cbfunc(void(*cs_sel)(void), void(*cs_desel)(void));
void reg_wizchip_spi_cbfunc(uint8_t (*spi_rb)(void), void (*spi_wb)(uint8_t wb));
void reg_wizchip_spiburst_cbfunc(void (*spi_rb)(uint8_t* pBuf, uint16_t len), void (*spi_wb)(uint8_t* pBuf, uint16_t len));

/**
 * @brief Configures W5500
 * @param module SPI module
 * @return None(constructor)
 */
W5500_Controller::W5500_Controller(mcu::SpiModule module, SPI_TransferProtocol protocol, SPI_Mode mode,
								uint32_t bitrate, uint16_t data_width, mcu::SpiTeMode te_mode,
								wiz_NetInfo* netinfo, W5500_UdpSettings udp_settings)
	: spi_(module, protocol, mode, bitrate, data_width, te_mode)
	, udp_(udp_settings)
{
	W5500_Controller::spi_base_ = spi_.GetBase();
	W5500_Controller::cs_pin_ = spi_.GetTePin();

	reg_wizchip_cs_cbfunc(W5500_Controller::ChipSelect, W5500_Controller::ChipDeselect);
	reg_wizchip_spi_cbfunc(W5500_Controller::ReadByte, W5500_Controller::WriteByte);
	reg_wizchip_spiburst_cbfunc(W5500_Controller::ReadBuff, W5500_Controller::WriteBuff);

	uint8_t rx_tx_buff_sizes[] = {2, 2, 2, 2, 2, 2, 2, 2};
	wizchip_init(rx_tx_buff_sizes, rx_tx_buff_sizes);
	wizchip_setnetinfo(netinfo);

	int8_t socket_in = socket(udp_.socket_rx, Sn_MR_UDP, udp_.port_rx, 0);
	int8_t socket_out = socket(udp_.socket_tx, Sn_MR_UDP, udp_.port_tx, 0);
}

/**
 * @brief
 * @param
 * @return
 */
int32_t W5500_Controller::Send(uint8_t* buff, uint16_t len)
{
	return sendto(udp_.socket_tx, buff, len, udp_.ip_tx, udp_.port_tx);
}

/*******************************************************************************
 * PRIVATE SPI-W5500 INTERFACE *
 ******************************************************************************/
/**
 * @brief
 * @param
 * @return
 */
void W5500_Controller::ChipSelect()
{
	GPIO_writePin(W5500_Controller::cs_pin_, 0);
}

/**
 * @brief
 * @param
 * @return
 */
void W5500_Controller::ChipDeselect()
{
	GPIO_writePin(W5500_Controller::cs_pin_, 1);
}

/**
 * @brief
 * @param
 * @return
 */
void W5500_Controller::WriteByte(uint8_t data)
{

	SPI_writeDataBlockingFIFO(W5500_Controller::spi_base_, data << 8);
	SPI_readDataBlockingFIFO(W5500_Controller::spi_base_);	// clear FIFO
}

/**
 * @brief
 * @param
 * @return
 */
uint8_t W5500_Controller::ReadByte()
{
	SPI_writeDataBlockingFIFO(W5500_Controller::spi_base_, 0xAAAA);	// send dummy data on TX line and generate clock on CLK line
	return SPI_readDataBlockingFIFO(W5500_Controller::spi_base_) & 0x00FF;
}

/**
 * @brief
 * @param
 * @return
 */
void W5500_Controller::WriteBuff(uint8_t* p_buff, uint16_t len)
{
	for (size_t i = 0; i < len; ++i)
	{
		SPI_writeDataBlockingFIFO(W5500_Controller::spi_base_, p_buff[i] << 8);
		SPI_readDataBlockingFIFO(W5500_Controller::spi_base_);	// clear FIFO
	}
}

/**
 * @brief
 * @param
 * @return
 */
void W5500_Controller::ReadBuff(uint8_t* p_buff, uint16_t len)
{
	if (len <= 16)
	{
		for (size_t i = 0; i < len; ++i)
		{
			SPI_writeDataBlockingFIFO(W5500_Controller::spi_base_, 0xAAAA);	// send dummy data on TX line and generate clock on CLK line
		}
		for (size_t i = 0; i < len; ++i)
		{
			p_buff[i] = SPI_readDataBlockingFIFO(W5500_Controller::spi_base_) & 0x00FF;
		}
	}
	else
	{
		for (size_t i = 0; i < len; ++i)
		{
			p_buff[i] = W5500_Controller::ReadByte();
		}
	}

}


