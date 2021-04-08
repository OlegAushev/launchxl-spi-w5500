/*
 * can_protocol.h
 *
 *  Created on: 30 мар. 2021 г.
 *      Author: Oleg Aushev
 */

#ifndef SRC_COMM_CAN_PROTOCOL_H_
#define SRC_COMM_CAN_PROTOCOL_H_

#include "driverlib.h"
#include "device.h"
#include <common.h>
#include <mcu/can/can_mcu.h>

namespace comm {

namespace CanTxMsgType {
enum CanTxMsgType
{
	TX,
	TIMESTAMP,
	DC_VOLTAGE,
	CURRENT,
	CURRENT_PHA,
	CURRENT_PHB,
	CURRENT_PHC,
	TEMPERATURE_PHA,
	TEMPERATURE_PHB,
	TEMPERATURE_PHC,
	TEMPERATURE_CASE,
	STATE,
	FAULT,
	CURRENT_D,
	CURRENT_Q,
	ANGLE,
	H_MAG,
	H_PHASE,
	CAN_TX_MSG_TOTAL_COUNT
};
} // namespace CanTxMsgType

namespace CanRxMsgType {
enum CanRxMsgType
{
	RX,
	SPEED,
	ANGLE,
	CAN_RX_MSG_TOTAL_COUNT
};
} // namespace CanRxMsgType

struct TxData
{
	float speed;
	uint32_t timestamp;
	float dc_voltage;
	float current;
	float current_phA;
	float current_phB;
	float current_phC;
	SimpleArray<float, 3> phase_currents;
	SimpleArray<float, 4> temperatures;
	uint32_t state;
	uint32_t fault_code;

	float current_d;
	float current_q;
	float angle;

	SimpleArray<float, 2> h_mag;
	SimpleArray<float, 2> h_phase;
};

struct RxData
{
	float speed;
	float angle;
	float float_value;
	CanRxMsgType::CanRxMsgType interrupt_cause;
};

void CanSend(mcu::Can *can, const TxData& data);

inline uint32_t GetCanMsgObjId(CanTxMsgType::CanTxMsgType type)
{
	return static_cast<uint32_t>(type) + 1;
}

inline uint32_t GetCanMsgObjId(CanRxMsgType::CanRxMsgType type)
{
	return 32 - static_cast<uint32_t>(type);
}

} /* namespace comm */



#endif /* SRC_COMM_CAN_PROTOCOL_H_ */
