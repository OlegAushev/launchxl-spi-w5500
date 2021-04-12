/*
 * adcx.cpp
 *
 *  Created on: 17 сент. 2020 г.
 *      Author: Oleg Aushev
 */

#include "adc_mcu.h"

/*__interrupt void ISR_ADC_CurrentPhaseAB();
__interrupt void ISR_ADC_CurrentPhaseC();
__interrupt void ISR_ADC_DcVoltage();

__interrupt void ISR_ADC_TemperatureACCase();
__interrupt void ISR_ADC_TemperatureC();*/


namespace mcu {

Adc* Adc::instance_ = static_cast<Adc*>(NULL);
const uint32_t Adc::module_bases_[3] = {ADCA_BASE, ADCB_BASE, ADCC_BASE};

/**
 * @brief Configures ADC
 * @param None
 * @return None(constructor)
 */
Adc::Adc()
{
	Adc::instance_ = this;

	for (uint32_t i = 0; i < 3; ++i)
	{
		ADC_setPrescaler(module_bases_[i], ADC_CLK_DIV_4_0);	// fclk(adc)max = 50 MHz
		ADC_setMode(module_bases_[i], ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);
		ADC_setInterruptPulseMode(module_bases_[i], ADC_PULSE_END_OF_CONV);
		ADC_enableConverter(module_bases_[i]);
		DEVICE_DELAY_US(1000);							// delay for power-up
	}

// Configure SOCs
// For 12-bit resolution, a sampling window of (5xADC_SAMPLE_WINDOW)ns at a 200MHz SYSCLK rate will be used
// Oversampled Conversion for currents and voltages

// SOC for A-Current: SOC0..3 on AA0
	ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_SW_ONLY,
					ADC_CH_ADCIN0, ADC_SAMPLE_WINDOW);
	ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_SW_ONLY,
					ADC_CH_ADCIN0, ADC_SAMPLE_WINDOW);
	ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_SW_ONLY,
					ADC_CH_ADCIN0, ADC_SAMPLE_WINDOW);
	ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER3, ADC_TRIGGER_SW_ONLY,
					ADC_CH_ADCIN0, ADC_SAMPLE_WINDOW);

// SOC for B-Current: SOC4..7 on AA2
	ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER4, ADC_TRIGGER_SW_ONLY,
					ADC_CH_ADCIN2, ADC_SAMPLE_WINDOW);
	ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER5, ADC_TRIGGER_SW_ONLY,
					ADC_CH_ADCIN2, ADC_SAMPLE_WINDOW);
	ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER6, ADC_TRIGGER_SW_ONLY,
					ADC_CH_ADCIN2, ADC_SAMPLE_WINDOW);
	ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER7, ADC_TRIGGER_SW_ONLY,
					ADC_CH_ADCIN2, ADC_SAMPLE_WINDOW);

// SOC for C-current: SOC0..3 on AB2
	ADC_setupSOC(ADCB_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_SW_ONLY,
					ADC_CH_ADCIN2, ADC_SAMPLE_WINDOW);
	ADC_setupSOC(ADCB_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_SW_ONLY,
					ADC_CH_ADCIN2, ADC_SAMPLE_WINDOW);
	ADC_setupSOC(ADCB_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_SW_ONLY,
					ADC_CH_ADCIN2, ADC_SAMPLE_WINDOW);
	ADC_setupSOC(ADCB_BASE, ADC_SOC_NUMBER3, ADC_TRIGGER_SW_ONLY,
					ADC_CH_ADCIN2, ADC_SAMPLE_WINDOW);

// SOC for DC-voltage: SOC0..3 on AC3
	ADC_setupSOC(ADCC_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_SW_ONLY,
					ADC_CH_ADCIN3, ADC_SAMPLE_WINDOW);
	ADC_setupSOC(ADCC_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_SW_ONLY,
					ADC_CH_ADCIN3, ADC_SAMPLE_WINDOW);
	ADC_setupSOC(ADCC_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_SW_ONLY,
					ADC_CH_ADCIN3, ADC_SAMPLE_WINDOW);
	ADC_setupSOC(ADCC_BASE, ADC_SOC_NUMBER3, ADC_TRIGGER_SW_ONLY,
					ADC_CH_ADCIN3, ADC_SAMPLE_WINDOW);

// Burst/priority config
	ADC_setSOCPriority(ADCA_BASE, ADC_PRI_ALL_HIPRI);	// all SOCs on ADCA at high priority - easier to control order
	ADC_setSOCPriority(ADCB_BASE, ADC_PRI_ALL_HIPRI);	// all SOCs on ADCB at high priority - easier to control order
	ADC_setSOCPriority(ADCC_BASE, ADC_PRI_ALL_HIPRI);	// all SOCs on ADCC at high priority - easier to control order

// Interrupt config
	// A&B-current interrupt - INT_ADCA1
	ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER7);
	ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER1);
	ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
	Interrupt_register(INT_ADCA1, Adc::OnCurrent_AB_Interrupt);

	// C-current interrupt - INT_ADCB1
	ADC_setInterruptSource(ADCB_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER3);
	ADC_enableInterrupt(ADCB_BASE, ADC_INT_NUMBER1);
	ADC_clearInterruptStatus(ADCB_BASE, ADC_INT_NUMBER1);
	Interrupt_register(INT_ADCB1, Adc::OnCurrent_C_Interrupt);

	// DC-voltage interrupt - INT_ADCC1
	ADC_setInterruptSource(ADCC_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER3);
	ADC_enableInterrupt(ADCC_BASE, ADC_INT_NUMBER1);
	ADC_clearInterruptStatus(ADCC_BASE, ADC_INT_NUMBER1);
	Interrupt_register(INT_ADCC1, Adc::OnVoltage_DC_Interrupt);

	for (uint32_t i = 0; i < 3; ++i)
	{
		zero_current_values_[i] = 0;
	}

/*=====================================================================================================*/
// SOC for Temperature sensing on AA4(phaseA), AC4(phaseB), AA5(phaseC), A15(case)
	ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER13, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN4, ADC_SAMPLE_WINDOW);
	ADC_setupSOC(ADCC_BASE, ADC_SOC_NUMBER15, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN4, ADC_SAMPLE_WINDOW);
	ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER14, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN5, ADC_SAMPLE_WINDOW);
	ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER15, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN15, ADC_SAMPLE_WINDOW);
//Temperature sensing interrupt config
	// A,C,case-temperature interrupt - INT_ADCA4
	ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER4, ADC_SOC_NUMBER15);
	ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER4);
	ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER4);
	Interrupt_register(INT_ADCA4, Adc::OnTemp_ACCase_Interrupt);

	// B-temperature interrupt - INT_ADCA4
	ADC_setInterruptSource(ADCC_BASE, ADC_INT_NUMBER4, ADC_SOC_NUMBER15);
	ADC_enableInterrupt(ADCC_BASE, ADC_INT_NUMBER4);
	ADC_clearInterruptStatus(ADCC_BASE, ADC_INT_NUMBER4);
	Interrupt_register(INT_ADCC4, Adc::OnTemp_B_Interrupt);
}


/**
 * @brief Enables ADC interrupts
 * @param None
 * @return None
 */
void Adc::EnableInterrupts() const
{
	Interrupt_enable(INT_ADCA1);
	Interrupt_enable(INT_ADCB1);
	Interrupt_enable(INT_ADCC1);
	Interrupt_enable(INT_ADCA4);
	Interrupt_enable(INT_ADCC4);
}


/**
 * @brief Calibrate ADC current measurements
 * @param None
 * @return
 */
void Adc::Calibrate(uint32_t number_of_meas)
{
	float sum[3];
	sum[Phase::A] = 0;
	sum[Phase::B] = 0;
	sum[Phase::C] = 0;

	for (uint32_t i = 0; i < number_of_meas; ++i)
	{
		this->StartPhaseCurrentsConv();
		while ((ADC_getInterruptStatus(ADCB_BASE, ADC_INT_NUMBER1) == false)
				&&(ADC_getInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1) == false))
		{}
		sum[Phase::A] += this->GetCurrentPhaseA();
		sum[Phase::B] += this->GetCurrentPhaseB();
		sum[Phase::C] += this->GetCurrentPhaseC();
		DEVICE_DELAY_US(1);
	}

	zero_current_values_[Phase::A] = sum[Phase::A] / number_of_meas;
	zero_current_values_[Phase::B] = sum[Phase::B] / number_of_meas;
	zero_current_values_[Phase::C] = sum[Phase::C] / number_of_meas;
}


} /* namespace uc */
