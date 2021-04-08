/*
 * can_mcu.cpp
 *
 *  Created on: 2 дек. 2020 г.
 *      Author: Oleg Aushev
 */
#include "can_mcu.h"
#include <comm/can/can_comm_msgs.h>

__interrupt void ISR_CANA_RX();

namespace mcu {

const uint32_t Can::reg_bases_[2] = {CANA_BASE, CANB_BASE};
const uint32_t Can::tx_pins_[2] = {19, 12};
const uint32_t Can::tx_pin_configs_[2] = {GPIO_19_CANTXA, GPIO_12_CANTXB};
const uint32_t Can::rx_pins_[2] = {18, 17};
const uint32_t Can::rx_pin_configs_[2] = {GPIO_18_CANRXA, GPIO_17_CANRXB};

#ifdef CPU1
/**
 * @brief Configures CAN module GPIO to be controlled by CPU2
 * @param can CAN module
 * @return None
 */
void Can::TransferControlToCpu2(CanModule can)
{
	GPIO_setPinConfig(rx_pin_configs_[can]);
	GPIO_setMasterCore(rx_pins_[can], GPIO_CORE_CPU2);
	GPIO_setPinConfig(tx_pin_configs_[can]);
	GPIO_setMasterCore(tx_pins_[can], GPIO_CORE_CPU2);
	SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL8_CAN, static_cast<uint16_t>(can)+1, SYSCTL_CPUSEL_CPU2);
}
#endif

/**
 * @brief Configures CAN module
 * @param can CAN module
 * @return None(constructor)
 */
Can::Can(CanModule can, CanBitrate bitrate)
	: reg_base_(reg_bases_[can])
{
#ifdef CPU1
	GPIO_setPinConfig(rx_pin_configs_[can]);
	GPIO_setPinConfig(tx_pin_configs_[can]);
#endif

	CAN_initModule(reg_base_);
	CAN_selectClockSource(reg_base_, CAN_CLOCK_SOURCE_SYS);

	switch (bitrate)
	{
	case CAN_BITRATE_125K:
		CAN_setBitRate(reg_base_, DEVICE_SYSCLK_FREQ, 125000, 16);
		break;
	case CAN_BITRATE_500K:
		CAN_setBitRate(reg_base_, DEVICE_SYSCLK_FREQ, 500000, 16);
		break;
	case CAN_BITRATE_1M:
		CAN_setBitRate(reg_base_, DEVICE_SYSCLK_FREQ, 1000000, 10);
		break;
	}

	CAN_disableRetry(reg_base_);

	for (int i = 0; i < tx_msg_count_; ++i)
	{
		CAN_setupMessageObject(reg_base_, tx_msg_[i].obj_id, tx_msg_[i].id, tx_msg_[i].frame,
							   tx_msg_[i].type, tx_msg_[i].id_mask, tx_msg_[i].flags, tx_msg_[i].len);
	}
	for (int i = 0; i < rx_msg_count_; ++i)
	{
		CAN_setupMessageObject(reg_base_, rx_msg_[i].obj_id, rx_msg_[i].id, rx_msg_[i].frame,
							   rx_msg_[i].type, rx_msg_[i].id_mask, rx_msg_[i].flags, rx_msg_[i].len);
	}

	if (reg_base_ == CANA_BASE)
	{
		Interrupt_register(INT_CANA0, &ISR_CANA_RX);
		CAN_enableInterrupt(reg_base_, CAN_INT_IE0 | CAN_INT_ERROR | CAN_INT_STATUS);
		Interrupt_enable(INT_CANA0);
		CAN_enableGlobalInterrupt(reg_base_, CAN_GLOBAL_INT_CANINT0);
	}
	CAN_startModule(reg_base_);
}

/*
 * @brief
 * @param
 * @return
 */
bool Can::Receive(size_t msg_type)
{
	if (CAN_readMessage(reg_base_, rx_msg_[msg_type].obj_id, rx_msg_[msg_type].data))
	{
		return true;
	}
	return false;
}


} /* namespace mcu */
