/*
 * adcx.h
 *
 *  Created on: 17 сент. 2020 г.
 *      Author: Oleg Aushev
 */

#ifndef DEVICE_ADCX_H_
#define DEVICE_ADCX_H_

#include "driverlib.h"
#include "device.h"
#include <common.h>

#define ADC_SAMPLE_WINDOW 60


namespace mcu {

class Adc
{
private:
	static const uint32_t reg_bases_[3];
	float zero_current_values_[3];

	Adc(const Adc& other);				// no copy constructor
	Adc& operator=(const Adc& other);	// no copy assignment operator

public:
	Adc();
	void Calibrate(uint32_t number_of_meas);
	void EnableInterrupts() const;

	void StartPhaseCurrentsConv() const
	{
		ADC_forceMultipleSOC(ADCB_BASE,
							 ADC_FORCE_SOC0 | ADC_FORCE_SOC1 | ADC_FORCE_SOC2 | ADC_FORCE_SOC3);
		ADC_forceMultipleSOC(ADCA_BASE,
							 ADC_FORCE_SOC0 | ADC_FORCE_SOC1 | ADC_FORCE_SOC2 | ADC_FORCE_SOC3 |
							 ADC_FORCE_SOC4 | ADC_FORCE_SOC5 | ADC_FORCE_SOC6 | ADC_FORCE_SOC7);
	}

	void StartDcVoltageConv() const
	{
		ADC_forceMultipleSOC(ADCC_BASE,
							 ADC_FORCE_SOC0 | ADC_FORCE_SOC1 | ADC_FORCE_SOC2 | ADC_FORCE_SOC3);
	}

	float GetCurrentPhaseA() const
	{
		uint32_t raw_data;
		raw_data = (ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0) +
					ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER1) +
					ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER2) +
					ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER3)) >> 2;

		float current = 800.f * ((float)raw_data / 4095.f) - 400.f - zero_current_values_[Phase::A];
		return current;
	}

	float GetCurrentPhaseB() const
	{
		uint32_t raw_data;
		raw_data = (ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER4) +
					ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER5) +
					ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER6) +
					ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER7)) >> 2;

		float current = 800.f * ((float)raw_data / 4095.f) - 400.f - zero_current_values_[Phase::B];
		return current;
	}

	float GetCurrentPhaseC() const
	{
		uint32_t raw_data;
		raw_data = (ADC_readResult(ADCBRESULT_BASE, ADC_SOC_NUMBER0) +
					ADC_readResult(ADCBRESULT_BASE, ADC_SOC_NUMBER1) +
					ADC_readResult(ADCBRESULT_BASE, ADC_SOC_NUMBER2) +
					ADC_readResult(ADCBRESULT_BASE, ADC_SOC_NUMBER3)) >> 2;

		float current = 800.f * ((float)raw_data / 4095.f) - 400.f - zero_current_values_[Phase::C];
		return current;
	}

	float GetDcVoltage() const
	{
		uint32_t raw_data;
		raw_data = (ADC_readResult(ADCCRESULT_BASE, ADC_SOC_NUMBER0) +
					ADC_readResult(ADCCRESULT_BASE, ADC_SOC_NUMBER1) +
					ADC_readResult(ADCCRESULT_BASE, ADC_SOC_NUMBER2) +
					ADC_readResult(ADCCRESULT_BASE, ADC_SOC_NUMBER3)) >> 2;
		float dc_voltage = 1200.f * ((float)raw_data / 4095.f);
		return dc_voltage;
	}
/*============================================================================*/
/*======================= Temperature methods ================================*/
/*============================================================================*/
	void StartTemperatureConv() const
	{
		ADC_forceMultipleSOC(ADCA_BASE, ADC_FORCE_SOC13 | ADC_FORCE_SOC14 | ADC_FORCE_SOC15);
		ADC_forceSOC(ADCC_BASE, ADC_SOC_NUMBER15);
	}

	float GetTemperaturePhaseA() const
	{
		const float beta = 3435;
	    float result, res, volt;
	    volt = 3.f * ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER13) / 4095.f;
	    res = 31937 * expf(-2.344f * volt);
	    result = 1.f / ((1/298.15f)+(logf(res/4700.f)/beta));
	    return result;
	}

	float GetTemperaturePhaseB() const
	{
		const float beta = 3435;
	    float result, res, volt;
	    volt = 3.f * ADC_readResult(ADCCRESULT_BASE, ADC_SOC_NUMBER15) / 4095.f;
	    res = 31937 * expf(-2.344f * volt);
	    result = 1.f / ((1/298.15f)+(logf(res/4700.f)/beta));
	    return result;
	}

	float GetTemperaturePhaseC() const
	{
		const float beta = 3435;
	    float result, res, volt;
	    volt = 3.f * ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER14) / 4095.f;
	    res = 31937 * expf(-2.344f * volt);
	    result = 1.f / ((1/298.15f)+(logf(res/4700.f)/beta));
	    return result;
	}

	float GetTemperatureCase() const
	{
	    float32_t volt, result;
	    volt = 3.f * ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER15) / 4095.f;
	    result = 1.f / ((logf(3.3f/volt - 1.f)/3900)+(1.f/298.15f));
	    return result;
	}
};

} /* namespace uc */


#endif /* DEVICE_ADCX_H_ */
