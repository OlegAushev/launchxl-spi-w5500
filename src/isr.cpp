
/*
 * ISRs.cpp
 *
 *  Created on: 18 сент. 2020 г.
 *      Author: Oleg Aushev
 */
#define USE_PROFILER

#include "F28x_Project.h"
#include "F2837xD_Ipc_drivers.h"
#include "device.h"
#include <math.h>
#include <profiler/profiler.h>
#include <mcu/cpu_timers/cpu_timers_mcu.h>

/*============================================================================*/
/*============================ CPU TIMERS ISRs ===============================*/
/*============================================================================*/
/*
 * @brief CPU Timer 0 ISR
 * @param None
 * @return None
 */
__interrupt void mcu::Clock::OnInterrupt()
{
	mcu::Clock::Instance()->Tick();
	if (mcu::Clock::Instance()->IsFlagSet(mcu::TIMER_FLAG_1))
	{
		mcu::Clock::Instance()->LedToggle(mcu::LED_BLUE);
		mcu::Clock::Instance()->LedToggle(mcu::LED_RED);
	}

	Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}


/*
 * @brief CPU Timer 1 ISR
 * @param None
 * @return None
 */
__interrupt void mcu::SystickISR()
{}

