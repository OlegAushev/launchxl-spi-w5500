/*
 * profiler.cpp
 *
 *  Created on: 27 нояб. 2020 г.
 *      Author: Oleg Aushev
 */

#include "profiler.h"


/**
 * @brief Configures GPIO pin as output to use LogDuration
 * @param pin GPIO pin number
 * @return None
 */
void ConfigureLogPin(uint32_t pin, GPIO_CoreSelect core)
{
	GPIO_setPadConfig(pin, GPIO_PIN_TYPE_STD);
	GPIO_setDirectionMode(pin, GPIO_DIR_MODE_OUT);
	GPIO_setMasterCore(pin, core);
}


LogData LogDurationSystick::durations_[log_size_];


uint32_t DefaultSystickFunc()
{
    return 0;
}

uint32_t (*LogDurationSystick::SystickFunc)() = DefaultSystickFunc;
