
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
__interrupt void ISR_CPU_TIMER0_Clock()
{
	extern mcu::Clock* h_clock;

	h_clock->Tick();
	if (h_clock->IsFlagSet(mcu::TIMER_FLAG_1))
	{
		h_clock->LedToggle(mcu::LED_BLUE);
		h_clock->LedToggle(mcu::LED_RED);
	}

	Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}


/*
 * @brief CPU Timer 1 ISR
 * @param None
 * @return None
 */
__interrupt void ISR_CPU_TIMER1_Systick()
{
	extern mcu::Clock* h_clock;
}

