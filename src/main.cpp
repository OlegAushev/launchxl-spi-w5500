
/*=====================*/
#define USE_PROFILER

#include "F28x_Project.h"
#include "F2837xD_Ipc_drivers.h"
#include "device.h"
#include <math.h>
#include <profiler/profiler.h>
#include <mcu/cpu_timers/cpu_timers_mcu.h>
#include <mcu/spi/spi_mcu.h>
#include <comm/w5500_controller/w5500_controller.h>
#include "socket.h"
#include "stdio.h"

#define SOCKET_RX	0
#define PORT_RX 	3000
#define SOCKET_TX 	1
#define PORT_TX 	3001

#define EDGE_IP		{192, 168, 70, 241}
#define W5500_IP	{192, 168, 70, 251}
#define W5500_GW	{192, 168, 70, 1}

wiz_NetInfo wiz_netinfo = {
		.mac = {0x00, 0x08, 0xdc, 0xab, 0xcd, 0xfe},
		.ip = W5500_IP,
		.sn = {255, 255, 255, 0},
		.gw = W5500_GW,
		.dns = {0, 0, 0, 0},
		.dhcp = NETINFO_STATIC };

W5500_UdpSettings udp_settings = {
		.socket_rx = SOCKET_RX,
		.port_rx = PORT_RX,
		.socket_tx = SOCKET_TX,
		.port_tx = PORT_TX,
		.ip_tx = EDGE_IP
};

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
	W5500_Controller w5500(mcu::SPIA, SPI_PROT_POL0PHA1, SPI_MODE_MASTER, 6250000, 8, mcu::SPI_TE_SW,
			&wiz_netinfo, udp_settings);
	/* INTERRUPTS */
	EINT;	// Enable Global interrupt INTM
	ERTM;	// Enable Global realtime interrupt

	while (true)
	{
		DEVICE_DELAY_US(1E6);
		char message_tx[128];
		sprintf(message_tx, "Current time is %lu.%lu\n", clock.GetSec(), clock.GetMilliSec());
		w5500.Send((uint8_t*)message_tx, strlen(message_tx));

		char message_rx[128] = {0};
		recvfrom(SOCKET_RX, (uint8_t*)message_rx, 128, NULL, &udp_settings.port_rx);
		w5500.Send((uint8_t*)message_rx, strlen(message_rx));

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

