/*
 * can_mcu.cpp
 *
 *  Created on: 2 дек. 2020 г.
 *      Author: Oleg Aushev
 */
#include "can_mcu.h"
#include <comm/can/can_comm_msgs.h>


namespace mcu {

Can* Can::instance_[2];
const uint32_t Can::module_bases_[2] = {CANA_BASE, CANB_BASE};
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
Can::Can(CanModule module, CanBitrate bitrate)
	: module_base_(module_bases_[module])
{
	Can::instance_[module] = this;
#ifdef CPU1
	GPIO_setPinConfig(rx_pin_configs_[module]);
	GPIO_setPinConfig(tx_pin_configs_[module]);
#endif

	CAN_initModule(module_base_);
	CAN_selectClockSource(module_base_, CAN_CLOCK_SOURCE_SYS);

	switch (bitrate)
	{
	case CAN_BITRATE_125K:
		CAN_setBitRate(module_base_, DEVICE_SYSCLK_FREQ, 125000, 16);
		break;
	case CAN_BITRATE_500K:
		CAN_setBitRate(module_base_, DEVICE_SYSCLK_FREQ, 500000, 16);
		break;
	case CAN_BITRATE_1M:
		CAN_setBitRate(module_base_, DEVICE_SYSCLK_FREQ, 1000000, 10);
		break;
	}

	CAN_disableRetry(module_base_);

	for (int i = 0; i < tx_msg_count_; ++i)
	{
		CAN_setupMessageObject(module_base_, tx_msg_[i].obj_id, tx_msg_[i].id, tx_msg_[i].frame,
							   tx_msg_[i].type, tx_msg_[i].id_mask, tx_msg_[i].flags, tx_msg_[i].len);
	}
	for (int i = 0; i < rx_msg_count_; ++i)
	{
		CAN_setupMessageObject(module_base_, rx_msg_[i].obj_id, rx_msg_[i].id, rx_msg_[i].frame,
							   rx_msg_[i].type, rx_msg_[i].id_mask, rx_msg_[i].flags, rx_msg_[i].len);
	}

	if (module_base_ == CANA_BASE)
	{
		Interrupt_register(INT_CANA0, Can::OnRxInterrupt);
		CAN_enableInterrupt(module_base_, CAN_INT_IE0 | CAN_INT_ERROR | CAN_INT_STATUS);
		Interrupt_enable(INT_CANA0);
		CAN_enableGlobalInterrupt(module_base_, CAN_GLOBAL_INT_CANINT0);
	}
	CAN_startModule(module_base_);
}

/*
 * @brief
 * @param
 * @return
 */
bool Can::Recv(size_t msg_type)
{
	if (CAN_readMessage(module_base_, rx_msg_[msg_type].obj_id, rx_msg_[msg_type].data))
	{
		return true;
	}
	return false;
}


} /* namespace mcu */
