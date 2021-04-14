
/*=====================*/
#define USE_PROFILER

#include "F28x_Project.h"
#include "F2837xD_Ipc_drivers.h"
#include "device.h"
#include <math.h>
#include <profiler/profiler.h>
#include <mcu/cpu_timers/cpu_timers_mcu.h>
#include <mcu/spi/spi_mcu.h>
#include <comm/controller_w5500/controller_w5500.h>
#include "socket.h"

void Foo() {}
void Bar() {}

/**
 * @brief main()
 * @param None
 * @return None
 */
void main()
{
	/* SYS */
	Device_init();					// Initializes system control, device clock, and peripherals
	Device_initGPIO();				// Configures the GPIO pin as a push-pull output

	LogDurationSystick::SetSystickFunc(mcu::GetSystick);
	ConfigureLogPin(22, GPIO_CORE_CPU1);
	ConfigureLogPin(67, GPIO_CORE_CPU1);

	Interrupt_initModule();			// Initializes PIE and clear PIE registers. Disables CPU interrupts.
    								// Clears all CPU interrupt flags.
	Interrupt_initVectorTable();	// Initialize the PIE vector table with pointers to the shell interrupt
    								// Service Routines (ISR).

	/* CLOCK */
	mcu::Clock clock(1, GPIO_CORE_CPU1, GPIO_CORE_CPU1);
	clock.LedOff(mcu::LED_BLUE);
	clock.SetFlagPeriod(mcu::TIMER_FLAG_1, 500);
	clock.SetFlagPeriod(mcu::TIMER_FLAG_2, 2);
	clock.SetFlagPeriod(mcu::TIMER_FLAG_3, 100);
	clock.SetFlagPeriod(mcu::TIMER_FLAG_4, 1000);
	mcu::ConfigureSystick();

	/* SPI */
	SpiW5500 spia(mcu::SPIA, SPI_PROT_POL0PHA0, SPI_MODE_MASTER, 500000, 8, mcu::SPI_TE_SW);

	/* INTERRUPTS */
	EINT;	// Enable Global interrupt INTM
	ERTM;	// Enable Global realtime interrupt

	uint8_t buff[] = {0xAB, 0x12, 0x34, 0x78};

	while (true)
	{
		//spia.SendNonBlocking(0xA0F0);
		//spia.SendBlockingNonFifo(0xBCDE);
		//SpiW5500::WriteByte(0xAD);
		//SpiW5500::WriteBuff(buff, 4);
		DEVICE_DELAY_US(1000);
	}
}


uint32_t g_error = 0;
/**
 * @brief Error handler
 * @param None
 * @return None
 */
void ErrorHandler(uint32_t error_code)
{
	g_error = error_code;
	while (true)
	{}
}

