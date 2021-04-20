
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
#include "stdio.h"

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
	SpiW5500 spia(mcu::SPIA, SPI_PROT_POL0PHA1, SPI_MODE_MASTER, 6250000, 8, mcu::SPI_TE_SW);
	/* INTERRUPTS */
	EINT;	// Enable Global interrupt INTM
	ERTM;	// Enable Global realtime interrupt

	int8_t socket_in = socket(0, Sn_MR_UDP, 3000, 0);
	int8_t socket_out = socket(1, Sn_MR_UDP, 3001, 0);
	printf("Hi!\n");



	while (true)
	{
		DEVICE_DELAY_US(1000);
		//send(1, (uint8_t*)"Hi from TI!\n", strlen("Hi from TI!\n"));
		uint8_t addr[4] = {192,168,70,241};
		char message[128];
		sprintf(message, "Current time is %lu%s%lu%s", clock.GetSec(), ".", clock.GetMilliSec(), "\n");
		sendto(1, (uint8_t*)message, strlen(message), addr, 3001);
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

