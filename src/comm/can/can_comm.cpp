/*
 * can_protocol.cpp
 *
 *  Created on: 30 мар. 2021 г.
 *      Author: Oleg Aushev
 */

#include "can_comm.h"


namespace comm {

void CanSend(mcu::Can *h_can, const TxData& data)
{
	h_can->Send<float>(CanTxMsgType::TX, data.speed);
	h_can->Send<uint32_t>(CanTxMsgType::TIMESTAMP, data.timestamp);
	h_can->Send<float>(CanTxMsgType::DC_VOLTAGE, data.dc_voltage);
	h_can->Send<float>(CanTxMsgType::CURRENT, data.current);
	h_can->Send<float>(CanTxMsgType::CURRENT_PHA, data.current_phA);
	h_can->Send<float>(CanTxMsgType::CURRENT_PHB, data.current_phB);
	h_can->Send<float>(CanTxMsgType::CURRENT_PHC, data.current_phC);
	h_can->Send<float>(CanTxMsgType::TEMPERATURE_PHA, data.temperatures[0]);
	h_can->Send<uint32_t>(CanTxMsgType::STATE, data.state);
	h_can->Send<uint32_t>(CanTxMsgType::FAULT, data.fault_code);

	h_can->Send<float>(CanTxMsgType::CURRENT_D, data.current_d);
	h_can->Send<float>(CanTxMsgType::CURRENT_Q, data.current_q);
	h_can->Send<float>(CanTxMsgType::ANGLE, data.angle);

	h_can->SendArray<float, 2>(CanTxMsgType::H_MAG, data.h_mag);
	h_can->SendArray<float, 2>(CanTxMsgType::H_PHASE, data.h_phase);
}



} /* namespace canprotocol */


