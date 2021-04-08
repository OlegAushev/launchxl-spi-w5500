/*
 * dac.h
 *
 *  Created on: 7 сент. 2020 г.
 *      Author: Oleg Aushev
 */

#ifndef INCLUDE_DAC_H_
#define INCLUDE_DAC_H_

#include "driverlib.h"
#include "device.h"


namespace mcu {

enum DacModule
{
	DACA,
	DACB
};

enum DacAModuleState
{
	DACA_OFF,
	DACA_ON
};

enum DacBModuleState
{
	DACB_OFF,
	DACB_ON
};


class Dac
{
private:
	static const uint32_t reg_bases_[2];
	float is_configured_[2];
	void Configure(DacModule dac);

public:
	Dac(DacAModuleState daca_state, DacBModuleState dacb_state);

	void SetOutput(DacModule dac, uint16_t value) const
	{
		if (is_configured_[dac])
		{
			DAC_setShadowValue(reg_bases_[dac], value);
		}
	}
};

} /* namespace uc */


#endif /* INCLUDE_DAC_H_ */
