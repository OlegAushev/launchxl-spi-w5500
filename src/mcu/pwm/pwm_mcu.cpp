/*
 * epwmx.cpp
 *
 *  Created on: 29 окт. 2020 г.
 *      Author: Oleg Aushev
 */

#include "pwm_mcu.h"


__interrupt void ISR_EPWM1();
__interrupt void ISR_EPWM_TZ();


namespace mcu {

const uint32_t Pwm::reg_bases_[3] = {EPWM1_BASE, EPWM2_BASE, EPWM3_BASE};


/**
 * @brief Initializes EPWM attributes, configures EPWM modules
 * @param fund_freq
 * @param sw_freq
 * @param dt
 * @return None(constructor)
 */
Pwm::Pwm(uint16_t fund_freq, uint16_t sw_freq, uint16_t dt)
	: fund_freq_(fund_freq)
	, sw_freq_(sw_freq)
	, dt_(dt)
	, is_stopped_by_sw_(false)
{
	// Configure GPIO
	/*
		0	AHS-PWM
		1	ALS-PWM
		2	BHS-PWM
		3	BLS-PWM
		4	CHS-PWM
		5	CLS-PWM
	*/
	GPIO_setPadConfig(0, GPIO_PIN_TYPE_STD);	// Enable push-pull on GPIO0
	GPIO_setPadConfig(1, GPIO_PIN_TYPE_STD);	// Enable push-pull on GPIO1
	GPIO_setPadConfig(2, GPIO_PIN_TYPE_STD);	// Enable push-pull on GPIO2
	GPIO_setPadConfig(3, GPIO_PIN_TYPE_STD);	// Enable push-pull on GPIO3
	GPIO_setPadConfig(4, GPIO_PIN_TYPE_STD);	// Enable push-pull on GPIO4
	GPIO_setPadConfig(5, GPIO_PIN_TYPE_STD);	// Enable push-pull on GPIO5
	GPIO_setPinConfig(GPIO_0_EPWM1A);	// GPIO0 = PWM1A
	GPIO_setPinConfig(GPIO_1_EPWM1B);	// GPIO1 = PWM1B
	GPIO_setPinConfig(GPIO_2_EPWM2A);	// GPIO2 = PWM2A
	GPIO_setPinConfig(GPIO_3_EPWM2B);	// GPIO3 = PWM2B
	GPIO_setPinConfig(GPIO_4_EPWM3A);	// GPIO4 = PWM3A
	GPIO_setPinConfig(GPIO_5_EPWM3B);	// GPIO5 = PWM3B

	/* 15 - Global Fault, TZ1 */
	GPIO_setPadConfig(15, GPIO_PIN_TYPE_PULLUP);	// Enable pullup on GPIO
	GPIO_setPinConfig(GPIO_15_GPIO15);				// GPIO = GPIO
	GPIO_setDirectionMode(15, GPIO_DIR_MODE_IN);	// GPIO = input
	GPIO_setQualificationMode(15, GPIO_QUAL_ASYNC);	// asynch input
	XBAR_setInputPin(XBAR_INPUT1, 15);				// GPIO15 = TZ1

	// Configure EPWM modules
	SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);	// Disable sync(Freeze clock to PWM as well)
	// Set-up TBCLK
	period_ = (200e6/sw_freq_)/2/2;	// there is a divider ( EPWMCLKDIV ) of the system clock
									// which defaults to EPWMCLK = SYSCLKOUT/2, fclk(epwm)max = 100 MHz
									// UP-DOWN count - divide by 2
	for (uint32_t i = 0; i < 3; ++i)
	{
		EPWM_setTimeBasePeriod(reg_bases_[i], period_);
		EPWM_setPhaseShift(reg_bases_[i], phase_[i]);
		EPWM_setTimeBaseCounter(reg_bases_[i], 0);
		if (reg_bases_[i] == EPWM1_BASE)
		{
			EPWM_setSyncOutPulseMode(reg_bases_[i], EPWM_SYNC_OUT_PULSE_ON_COUNTER_ZERO);	// module EPWM1 is master
		}
		else
		{
			EPWM_setSyncOutPulseMode(reg_bases_[i], EPWM_SYNC_OUT_PULSE_ON_EPWMxSYNCIN);	// module EPWM sync is pass-thru
		}

	// Set compare values
		EPWM_setCounterCompareValue(reg_bases_[i], EPWM_COUNTER_COMPARE_A, 0.5*period_);	// start with 50%

	// Set up counter mode
		EPWM_setTimeBaseCounterMode(reg_bases_[i], EPWM_COUNTER_MODE_UP_DOWN);
		if (reg_bases_[i] == EPWM1_BASE)
		{
			EPWM_disablePhaseShiftLoad(reg_bases_[i]); //master has no phase shift
		}
		else
		{
			EPWM_enablePhaseShiftLoad(reg_bases_[i]);
		}
		EPWM_setClockPrescaler(reg_bases_[i], EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_1);

	// Set up shadowing
		EPWM_setCounterCompareShadowLoadMode(reg_bases_[i], EPWM_COUNTER_COMPARE_A, EPWM_COMP_LOAD_ON_CNTR_ZERO);

	// Set actions
		EPWM_setActionQualifierAction(reg_bases_[i],
										EPWM_AQ_OUTPUT_A,
										EPWM_AQ_OUTPUT_HIGH,
										EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
		EPWM_setActionQualifierAction(reg_bases_[i],
										EPWM_AQ_OUTPUT_A,
										EPWM_AQ_OUTPUT_LOW,
										EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
		EPWM_setActionQualifierAction(reg_bases_[i],
										EPWM_AQ_OUTPUT_B,
										EPWM_AQ_OUTPUT_HIGH,
										EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
		EPWM_setActionQualifierAction(reg_bases_[i],
										EPWM_AQ_OUTPUT_B,
										EPWM_AQ_OUTPUT_LOW,
										EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);

	// Set Deadband Active High Complimentary
		EPWM_setDeadBandControlShadowLoadMode(reg_bases_[i], EPWM_DB_LOAD_ON_CNTR_ZERO);
		EPWM_setDeadBandDelayMode(reg_bases_[i], EPWM_DB_FED, true);
		EPWM_setDeadBandDelayMode(reg_bases_[i], EPWM_DB_RED, true);
		EPWM_setDeadBandDelayPolarity(reg_bases_[i], EPWM_DB_RED, EPWM_DB_POLARITY_ACTIVE_HIGH);
		EPWM_setDeadBandDelayPolarity(reg_bases_[i], EPWM_DB_FED, EPWM_DB_POLARITY_ACTIVE_LOW);
		EPWM_setRisingEdgeDeadBandDelayInput(reg_bases_[i], EPWM_DB_INPUT_EPWMA);
		EPWM_setFallingEdgeDeadBandDelayInput(reg_bases_[i], EPWM_DB_INPUT_EPWMA);
		EPWM_setRisingEdgeDelayCount(reg_bases_[i], dt_);
		EPWM_setFallingEdgeDelayCount(reg_bases_[i], dt_);
		EPWM_setDeadBandCounterClock(reg_bases_[i], EPWM_DB_COUNTER_CLOCK_FULL_CYCLE);

	// Enable TZ1 as one shot trip sources
		EPWM_enableTripZoneSignals(reg_bases_[i], EPWM_TZ_SIGNAL_OSHT1);
		EPWM_setTripZoneAction(reg_bases_[i],
								EPWM_TZ_ACTION_EVENT_TZA,
								EPWM_TZ_ACTION_LOW);
		EPWM_setTripZoneAction(reg_bases_[i],
								EPWM_TZ_ACTION_EVENT_TZB,
								EPWM_TZ_ACTION_LOW);
	}

	EPWM_enableTripZoneInterrupt(EPWM1_BASE, EPWM_TZ_INTERRUPT_OST);

	// Configure interrupt to change the Compare Values, only phase A interrupt is required
	//EPWM_setInterruptSource(EPWM1_BASE, EPWM_INT_TBCTR_ZERO);
	//EPWM_setInterruptSource(EPWM1_BASE, EPWM_INT_TBCTR_PERIOD);
	EPWM_setInterruptSource(EPWM1_BASE, EPWM_INT_TBCTR_ZERO_OR_PERIOD);
	EPWM_setInterruptEventCount(EPWM1_BASE, 1U);
	//EPWM_enableInterrupt(EPWM1_BASE);

	radian_ = 0;
	radian_step_ = 2 * M_PI / (sw_freq_ / fund_freq_);

	mod_factor_ = 0.8;

	phase_[0] = 0;
	phase_[1] = 0;
	phase_[2] = 0;

	SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);	// Enable sync and clock to PWM

	// Assign the interrupt service routines to ePWM interrupts
	Interrupt_register(INT_EPWM1, &ISR_EPWM1);
    Interrupt_register(INT_EPWM1_TZ, &ISR_EPWM_TZ);
	Interrupt_enable(INT_EPWM1);
	Interrupt_enable(INT_EPWM1_TZ);
	this->Stop();
}


} /* namespace uc */
