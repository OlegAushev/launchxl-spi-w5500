/*
 * spi_mcu.h
 *
 *  Created on: 8 апр. 2021 г.
 *      Author: Oleg Aushev
 */

#ifndef SRC_MCU_SPI_SPI_MCU_H_
#define SRC_MCU_SPI_SPI_MCU_H_

#include "driverlib.h"
#include "device.h"
#include <common.h>

namespace mcu {

enum SpiModule
{
	SPIA,
	SPIB,
	SPIC
};

class Spi
{
private:
	static const uint32_t module_bases_[3];
	static const uint32_t simo_pins_[3];
	static const uint32_t somi_pins_[3];
	static const uint32_t clk_pins_[3];
	static const uint32_t te_pins_[3];

	static const uint32_t simo_pin_configs_[3];
	static const uint32_t somi_pin_configs_[3];
	static const uint32_t clk_pin_configs_[3];
	static const uint32_t te_pin_configs_[3];

	const uint32_t module_base_;
public:
	Spi(SpiModule module, SPI_TransferProtocol protocol, SPI_Mode mode,
		uint32_t bitrate, uint16_t data_width);

	uint32_t GetBase() const { return module_base_; }

	void SendNonBlocking(uint16_t data);
	void SendBlockingNonFifo(uint16_t data);
};



} // namespace mcu


#endif /* SRC_MCU_SPI_SPI_MCU_H_ */
