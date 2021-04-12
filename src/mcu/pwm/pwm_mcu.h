/*
 * epwmx.h
 *
 *  Created on: 29 окт. 2020 г.
 *      Author: Oleg Aushev
 */

#ifndef INCLUDE_UC_PWM_H_
#define INCLUDE_UC_PWM_H_

#include "driverlib.h"
#include "device.h"
#include <common.h>
#include <math.h>


namespace mcu {

class Pwm
{
private:
	static const uint32_t module_bases_[3];

	uint16_t fund_freq_;
	uint16_t sw_freq_;
	uint16_t dt_;			// dead time in clock cycles

	uint16_t period_;		// TBPRD register value
	float radian_;			// current radian value for phase A
	float radian_step_;
	float mod_factor_;

	uint16_t phase_[3];		// TBPHS registers values

	bool is_stopped_by_sw_;

	Pwm(const Pwm& other);			// no copy constructor
	Pwm& operator=(const Pwm& other);	// no copy assignment operator

public:
	Pwm(uint16_t fund_freq, uint16_t sw_freq, uint16_t dt);
	void EnableInterrupts() const
	{
		EPWM_enableInterrupt(EPWM1_BASE);
	}

	void DisableInterrupts() const
	{
		EPWM_disableInterrupt(EPWM1_BASE);
	}

	float GetRadian() const {return radian_;}
	float GetRadianStep() const {return radian_step_;}
	float GetModFactor() const {return mod_factor_;}
	uint16_t GetPeriod() const {return period_;}

	/**
	 * @brief Updates counter compare values
	 * @param Sine values array pointer
	 * @return None
	 */
	void UpdateCmpValues(float sines[]) const
	{
		for (uint32_t i = 0; i < 3; ++i)
		{
			EPWM_setCounterCompareValue(module_bases_[i],
			                            EPWM_COUNTER_COMPARE_A,
			                            sines[i]*period_);
		}
	}

	/**
	 * @brief Updates counter compare values
	 * @param CCMP registers values array pointer
	 * @return None
	 */
	void UpdateCmpValues(uint16_t reg_values[]) const
	{
		for (uint32_t i = 0; i < 3; ++i)
		{
			EPWM_setCounterCompareValue(module_bases_[i],
			                            EPWM_COUNTER_COMPARE_A,
			                            reg_values[i]);
		}
	}

	/**
	 * @brief Updates counter compare values
	 * @param CCMP registers values struct
	 * @return None
	 */
	void UpdateCmpValues(const SimpleArray<uint16_t, 3>& reg_values) const
	{
		for (uint32_t i = 0; i < 3; ++i)
		{
			EPWM_setCounterCompareValue(module_bases_[i],
			                            EPWM_COUNTER_COMPARE_A,
			                            reg_values[i]);
		}
	}

	/**
	 * @brief Starts PWM
	 * @param None
	 * @return None
	 */
	void Start()
	{
		this->EnableInterrupts();
		for (uint32_t i = 0; i < 3; ++i)
		{
			EPWM_clearTripZoneFlag(module_bases_[i], EPWM_TZ_FLAG_OST);
			EPWM_clearTripZoneFlag(module_bases_[i], EPWM_TZ_INTERRUPT);
		}
		is_stopped_by_sw_ = false;
	}

	/**
	 * @brief Stops PWM
	 * @param None
	 * @return None
	 */
	void Stop()
	{
		is_stopped_by_sw_ = true;
		for (uint32_t i = 0; i < 3; ++i)
		{
			EPWM_forceTripZoneEvent(module_bases_[i], EPWM_TZ_FORCE_EVENT_OST);
		}
		this->DisableInterrupts();
	}

	bool IsStoppedBySofware() { return is_stopped_by_sw_; }
	static __interrupt void OnInterrupt();
	static __interrupt void OnTripInterrupt();
};

} /* namespace uc */


#endif /* INCLUDE_UC_PWM_H_ */
