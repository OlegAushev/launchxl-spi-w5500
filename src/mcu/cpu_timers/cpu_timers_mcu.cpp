/*
 * cpu_timers.c
 *
 *  Created on: 7 сент. 2020 г.
 *      Author: Oleg Aushev
 */

#include "cpu_timers_mcu.h"

/*
 * @brief CPU Timer 0 ISR prototype
 * @param None
 * @return None
 */
__interrupt void ISR_CPU_TIMER0_Clock();
__interrupt void ISR_CPU_TIMER1_Systick();


namespace mcu {

#ifdef CPU1
/**
 * @brief Configures CPU Timer0 as Clock
 * @param time_step_usec Interrupt period in usec
 * @param core1 CPU to control BLUE LED
 * @param core2 CPU to control RED LED
 * @return None(constructor)
 */
Clock::Clock(uint32_t time_step_msec, GPIO_CoreSelect core1, GPIO_CoreSelect core2)
	: msec_(0)
	, sec_(0)
	, time_step_msec_(time_step_msec)
{
	Interrupt_register(INT_TIMER0, ISR_CPU_TIMER0_Clock);

	CPUTimer_setPeriod(CPUTIMER0_BASE, 0xFFFFFFFF);	// Initialize timer period to maximum
	CPUTimer_setPreScaler(CPUTIMER0_BASE, 0);		// Initialize pre-scale counter to divide by 1 (SYSCLKOUT)
	CPUTimer_stopTimer(CPUTIMER0_BASE);				// Make sure timer is stopped
	CPUTimer_reloadTimerCounter(CPUTIMER0_BASE);	// Reload counter register with period value

    uint32_t tmp = (uint32_t)(DEVICE_SYSCLK_FREQ / 1000 * time_step_msec_);
    CPUTimer_setPeriod(CPUTIMER0_BASE, tmp);
	CPUTimer_setEmulationMode(CPUTIMER0_BASE, CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);

	GPIO_setPadConfig(31, GPIO_PIN_TYPE_STD);
	GPIO_writePin(31, 1);	// sic!
	GPIO_setPinConfig(GPIO_31_GPIO31);
	GPIO_setDirectionMode(31, GPIO_DIR_MODE_OUT);
	GPIO_setMasterCore(31, core1);

	GPIO_setPadConfig(34, GPIO_PIN_TYPE_STD);
	GPIO_writePin(34, 1);	// sic!
	GPIO_setPinConfig(GPIO_34_GPIO34);
	GPIO_setDirectionMode(34, GPIO_DIR_MODE_OUT);
	GPIO_setMasterCore(34, core2);

	for (int i = 0; i < 4; ++i)
	{
		flag_periods_[i] = 0x7FFFFFFF;
	}

	CPUTimer_enableInterrupt(CPUTIMER0_BASE);
	Interrupt_enable(INT_TIMER0);
	CPUTimer_startTimer(CPUTIMER0_BASE);
}
#endif
#ifdef CPU2
/**
 * @brief Configures CPU Timer0 as Clock
 * @param time_step_usec Interrupt period in usec
 * @return None(constructor)
 */
Clock::Clock(uint32_t time_step_msec)
	: msec_(0)
	, sec_(0)
	, time_step_msec_(time_step_msec)
{
	Interrupt_register(INT_TIMER0, ISR_CPU_TIMER0_Clock);

	CPUTimer_setPeriod(CPUTIMER0_BASE, 0xFFFFFFFF);	// Initialize timer period to maximum
	CPUTimer_setPreScaler(CPUTIMER0_BASE, 0);		// Initialize pre-scale counter to divide by 1 (SYSCLKOUT)
	CPUTimer_stopTimer(CPUTIMER0_BASE);				// Make sure timer is stopped
	CPUTimer_reloadTimerCounter(CPUTIMER0_BASE);	// Reload counter register with period value

    uint32_t tmp = (uint32_t)(DEVICE_SYSCLK_FREQ / 1000 * time_step_msec_);
    CPUTimer_setPeriod(CPUTIMER0_BASE, tmp);
	CPUTimer_setEmulationMode(CPUTIMER0_BASE, CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);

	for (int i = 0; i < 4; ++i)
	{
		flag_periods_[i] = 0x7FFFFFFF;
	}

	CPUTimer_enableInterrupt(CPUTIMER0_BASE);
	Interrupt_enable(INT_TIMER0);
	CPUTimer_startTimer(CPUTIMER0_BASE);
}
#endif

/**
 * @brief Resets Clock
 * @param None
 * @return None
 */
void Clock::Reset()
{
	msec_ = 0;
	sec_ = 0;
}

/**
 * @brief Resets timer and waits timer reset on other CPU
 * @param None
 * @return None
 */
void Clock::SyncWithOtherCpu(uint32_t ipc_flag)
{
	IPCLtoRFlagSet(ipc_flag);
	while (IPCLtoRFlagBusy(ipc_flag)) {}
	this->Reset();
}

/**
 * @brief Configures CPU Timer1 as SysTick
 * @param None
 * @return None
 */
void ConfigureSystick()
{
    Interrupt_register(INT_TIMER1, ISR_CPU_TIMER1_Systick);

    CPUTimer_setPeriod(CPUTIMER1_BASE, 0xFFFFFFFF);  // Initialize timer period to maximum
    CPUTimer_setPreScaler(CPUTIMER1_BASE, 0);       // Initialize pre-scale counter to divide by 1 (SYSCLKOUT)
    CPUTimer_stopTimer(CPUTIMER1_BASE);             // Make sure timer is stopped
    CPUTimer_reloadTimerCounter(CPUTIMER1_BASE);    // Reload counter register with period value

    uint32_t tmp = (uint32_t)(DEVICE_SYSCLK_FREQ / 1000000 * 1e6);
    CPUTimer_setPeriod(CPUTIMER1_BASE, tmp);
    CPUTimer_setEmulationMode(CPUTIMER1_BASE, CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);

    CPUTimer_enableInterrupt(CPUTIMER1_BASE);
    Interrupt_enable(INT_TIMER1);
    CPUTimer_startTimer(CPUTIMER1_BASE);
}

} /* namespace mcu */
