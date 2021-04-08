/*
 * profiler.h
 *
 *  Created on: 24 сент. 2020 г.
 *      Author: Oleg Aushev
 */

#ifndef INCLUDE_PROFILER_H_
#define INCLUDE_PROFILER_H_

#include "driverlib.h"
#include "device.h"


/**
 * @brief Configures GPIO pin as output to use LogDuration
 * @param pin GPIO pin number
 * @return None
 */
void ConfigureLogPin(uint32_t pin, GPIO_CoreSelect core);


class LogDurationPin
{
private:
	const uint32_t pin_;
public:
	explicit LogDurationPin(uint32_t pin)
	: pin_(pin)
	{
		GPIO_writePin(pin_, 1);
	}

	~LogDurationPin()
	{
		GPIO_writePin(pin_, 0);
	}
};


struct LogData
{
    const char* key;
    uint32_t value;
};

/**
 * @brief +50..100ns
 */
class LogDurationSystick
{
private:
    static uint32_t (*SystickFunc)();
    const static uint32_t log_size_ = 10;
    static LogData durations_[log_size_];
    const char* message_;
    const uint32_t cell_;
    uint32_t start_;
public:
    LogDurationSystick(const char* message, uint32_t cell)
    : message_(message)
    , cell_(cell)
    {
        start_ = SystickFunc();
    }

    ~LogDurationSystick()
    {
        uint32_t duration = start_ - SystickFunc();
        durations_[cell_].key = message_;
        durations_[cell_].value = duration;
    }


    static void SetSystickFunc(uint32_t (*Func)())
    {
        SystickFunc = Func;
    }
};



#define UNIQ_ID_IMPL(line) _a_local_var_##line
#define UNIQ_ID(line) UNIQ_ID_IMPL(line)

#ifdef USE_PROFILER
#define LOG_DURATION_PIN(pin) LogDurationPin UNIQ_ID(__LINE__)(pin);
#define LOG_DURATION_SYSTICK(message, cell) LogDurationSystick UNIQ_ID(__LINE__)(message, cell);
#else
#define LOG_DURATION_PIN(pin)
#define LOG_DURATION_SYSTICK(message, cell)
#endif


#endif /* INCLUDE_PROFILER_H_ */
