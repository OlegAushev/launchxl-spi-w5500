/*
 * dac.c
 *
 *  Created on: 7 сент. 2020 г.
 *      Author: Oleg Aushev
 */

#include "dac_mcu.h"


namespace mcu {

const uint32_t Dac::reg_bases_[2] = {DACA_BASE, DACB_BASE};


/**
 * @brief Configures DAC modules
 * @param DACA state
 * @param DACB state
 * @return None(constructor)
 */
Dac::Dac(DacAModuleState daca_state, DacBModuleState dacb_state) {
	if (daca_state == DACA_ON)
	{
		is_configured_[DACA] = true;
		this->Configure(DACA);
	}
	else
	{
		is_configured_[DACA] = false;
	}

	if (dacb_state == DACB_ON)
	{
		is_configured_[DACB] = true;
		this->Configure(DACB);
	}
	else
	{
		is_configured_[DACB] = false;
	}
}


/*
 * @brief Setup the DAC reference selection and enable specified DAC
 * @param DAC module
 * @return None
 */
void Dac::Configure(DacModule dac)
{
	DAC_setReferenceVoltage(reg_bases_[dac], DAC_REF_ADC_VREFHI);
	DAC_enableOutput(reg_bases_[dac]);
	DAC_setShadowValue(reg_bases_[dac], 0);
	DEVICE_DELAY_US(10);				// Delay for buffered DAC to power up
}

} /* namespace uc */
