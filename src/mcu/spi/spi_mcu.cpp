/*
 * spi_mcu.cpp
 *
 *  Created on: 8 апр. 2021 г.
 *      Author: Oleg Aushev
 */

#include "spi_mcu.h"

namespace mcu {

const uint32_t Spi::module_bases_[3] = {SPIA_BASE, SPIB_BASE, SPIC_BASE};

const uint32_t Spi::simo_pins_[3] =	{58, 63, 122};
const uint32_t Spi::somi_pins_[3] =	{59, 64, 123};
const uint32_t Spi::clk_pins_[3] =	{60, 65, 124};
const uint32_t Spi::te_pins_[3]	=	{61, 66, 125};

const uint32_t Spi::simo_pin_configs_[3] =	{GPIO_58_SPISIMOA, GPIO_63_SPISIMOB, GPIO_122_SPISIMOC};
const uint32_t Spi::somi_pin_configs_[3] = 	{GPIO_59_SPISOMIA, GPIO_64_SPISOMIB, GPIO_123_SPISOMIC};
const uint32_t Spi::clk_pin_configs_[3] = 	{GPIO_60_SPICLKA, GPIO_65_SPICLKB, GPIO_124_SPICLKC};
const uint32_t Spi::te_pin_configs_[3] = 	{GPIO_61_SPISTEA, GPIO_66_SPISTEB, GPIO_125_SPISTEC};
const uint32_t Spi::sw_te_pin_configs_[3] =	{GPIO_61_GPIO61, GPIO_66_GPIO66, GPIO_125_GPIO125};


/**
 * @brief Configures SPI module
 * @param module SPI module
 * @return None(constructor)
 */
Spi::Spi(SpiModule module, SPI_TransferProtocol protocol, SPI_Mode mode,
			uint32_t bitrate, uint16_t data_width, SpiTeMode te_mode)
	: module_(module)
	, module_base_(module_bases_[module])
	, data_width_(data_width)
{
	SPI_disableModule(module_base_);
	SPI_setConfig(module_base_, DEVICE_LSPCLK_FREQ, protocol, mode, bitrate, data_width);
	SPI_disableLoopback(module_base_);
	SPI_setEmulationMode(module_base_, SPI_EMULATION_FREE_RUN);


#ifdef CPU1	/* Only CPU1 can configure GPIO */
	GPIO_setPinConfig(simo_pin_configs_[module]);
	GPIO_setPadConfig(simo_pins_[module], GPIO_PIN_TYPE_PULLUP);
	GPIO_setQualificationMode(simo_pins_[module], GPIO_QUAL_ASYNC);

	GPIO_setPinConfig(somi_pin_configs_[module]);
	GPIO_setPadConfig(somi_pins_[module], GPIO_PIN_TYPE_PULLUP);
	GPIO_setQualificationMode(somi_pins_[module], GPIO_QUAL_ASYNC);

	GPIO_setPinConfig(clk_pin_configs_[module]);
	GPIO_setPadConfig(clk_pins_[module], GPIO_PIN_TYPE_PULLUP);
	GPIO_setQualificationMode(clk_pins_[module], GPIO_QUAL_ASYNC);

	switch (te_mode)
	{
	case SPI_TE_HW:
		GPIO_setPinConfig(te_pin_configs_[module]);
		GPIO_setPadConfig(te_pins_[module], GPIO_PIN_TYPE_PULLUP);
		GPIO_setQualificationMode(te_pins_[module], GPIO_QUAL_ASYNC);
		break;

	case SPI_TE_SW:
		GPIO_setPinConfig(sw_te_pin_configs_[module]);
		GPIO_setPadConfig(te_pins_[module], GPIO_PIN_TYPE_STD);
		GPIO_setDirectionMode(te_pins_[module], GPIO_DIR_MODE_OUT);
		GPIO_writePin(te_pins_[module], 1);
	}


#endif
	SPI_disableFIFO(module_base_);
	SPI_enableModule(module_base_);
	SPI_enableTalk(module_base_);
}







} // namespace mcu

