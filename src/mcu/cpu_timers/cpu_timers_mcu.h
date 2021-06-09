/*
 * cpu_timers.h
 *
 *  Created on: 7 сент. 2020 г.
 *      Author: Oleg Aushev
 */

#ifndef INCLUDE_UC_CPU_TIMERS_H_
#define INCLUDE_UC_CPU_TIMERS_H_

#include "F2837xD_Ipc_drivers.h"
#include "driverlib.h"
#include "device.h"


namespace mcu {

enum TimerFlag
{
	TIMER_FLAG_1,
	TIMER_FLAG2,
	TIMER_FLAG3,
	TIMER_FLAG4,
};

enum LaunchPadLed
{
	LED_BLUE = 31,
	LED_RED = 34
};

class Clock
{
private:
	static Clock* instance_;
	uint32_t msec_;
	uint32_t sec_;
	const uint32_t time_step_msec_;

	uint32_t flag_periods_[4];
	bool flags_[4];

	Clock(const Clock& other);				// no copy constructor
	Clock& operator=(const Clock& other);	// no copy assignment operator

public:
#ifdef CPU1
	Clock(uint32_t time_step_msec, GPIO_CoreSelect core1, GPIO_CoreSelect core2);
#endif
#ifdef CPU2
	Clock(uint32_t time_step_msec);
#endif

	static Clock* Instance() { return instance_; };

	void Tick()
	{
		msec_ += time_step_msec_;
		if (msec_ == 1000)
		{
			msec_ = 0;
			++sec_;
			if (sec_ == 3600)
			{
				sec_ = 0;
			}
		}

		for (int i = 0; i < 4; ++i)
		{
			if (this->GetTimestamp() % flag_periods_[i] == 0)
			{
				flags_[i] = true;
			}
		}
	}

	uint32_t GetTimestamp() const { return 1000 * sec_ + msec_; }
	uint32_t GetSec() const { return sec_; }
	uint32_t GetMilliSec() const { return msec_; }
	void Reset();

	void LedOn(LaunchPadLed led) const { GPIO_writePin(led, 0); }
	void LedOff(LaunchPadLed led) const { GPIO_writePin(led, 1); }
	void LedToggle(LaunchPadLed led) const { GPIO_togglePin(led); }

	void SetFlagPeriod(TimerFlag flag, uint32_t period) { flag_periods_[flag] = period; }
	bool IsFlagSet(TimerFlag flag)
	{
		if (flags_[flag] == true)
		{
			flags_[flag] = false;	// reset flag if it was true
			return true;
		}
		return false;
	}

	void SyncWithOtherCpu(uint32_t ipc_flag);
	static __interrupt void OnInterrupt();
};


void ConfigureSystick();
inline uint32_t GetSystick() {return CPUTimer_getTimerCount(CPUTIMER1_BASE);}
__interrupt void SystickISR();

} /* namespace mcu */


#endif /* INCLUDE_UC_CPU_TIMERS_H_ */
