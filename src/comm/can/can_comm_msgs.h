/*
 * can_comm_msgs.h
 *
 *  Created on: 31 мар. 2021 г.
 *      Author: Oleg Aushev
 */

#ifndef SRC_COMM_CAN_CAN_COMM_MSGS_H_
#define SRC_COMM_CAN_CAN_COMM_MSGS_H_
#include "can_comm.h"

const size_t mcu::Can::tx_msg_count_ = comm::CanTxMsgType::CAN_TX_MSG_TOTAL_COUNT;
const size_t mcu::Can::rx_msg_count_ = comm::CanRxMsgType::CAN_RX_MSG_TOTAL_COUNT;

mcu::CanMsg mcu::Can::tx_msg_[32] =
{
	/*----------------*/
	{
		.obj_id = static_cast<uint32_t>(comm::CanTxMsgType::TX) + 1,
		.id = 0x000000A0,
		.frame = CAN_MSG_FRAME_STD,
		.type = CAN_MSG_OBJ_TYPE_TX,
		.id_mask = 0,
		.flags = CAN_MSG_OBJ_NO_FLAGS,
		.len = 4
	},
	/*----------------*/
	{
		.obj_id = static_cast<uint32_t>(comm::CanTxMsgType::TIMESTAMP) + 1,
		.id = 0x000000A1,
		.frame = CAN_MSG_FRAME_STD,
		.type = CAN_MSG_OBJ_TYPE_TX,
		.id_mask = 0,
		.flags = CAN_MSG_OBJ_NO_FLAGS,
		.len = 4
	},
	/*----------------*/
	{
		.obj_id = static_cast<uint32_t>(comm::CanTxMsgType::DC_VOLTAGE) + 1,
		.id = 0x000000A2,
		.frame = CAN_MSG_FRAME_STD,
		.type = CAN_MSG_OBJ_TYPE_TX,
		.id_mask = 0,
		.flags = CAN_MSG_OBJ_NO_FLAGS,
		.len = 4
	},
	/*----------------*/
	{
		.obj_id = static_cast<uint32_t>(comm::CanTxMsgType::CURRENT) + 1,
		.id = 0x000000A3,
		.frame = CAN_MSG_FRAME_STD,
		.type = CAN_MSG_OBJ_TYPE_TX,
		.id_mask = 0,
		.flags = CAN_MSG_OBJ_NO_FLAGS,
		.len = 4
	},
	/*----------------*/
	{
		.obj_id = static_cast<uint32_t>(comm::CanTxMsgType::CURRENT_PHA) + 1,
		.id = 0x000000A4,
		.frame = CAN_MSG_FRAME_STD,
		.type = CAN_MSG_OBJ_TYPE_TX,
		.id_mask = 0,
		.flags = CAN_MSG_OBJ_NO_FLAGS,
		.len = 4
	},
	/*----------------*/
	{
		.obj_id = static_cast<uint32_t>(comm::CanTxMsgType::CURRENT_PHB) + 1,
		.id = 0x000000A5,
		.frame = CAN_MSG_FRAME_STD,
		.type = CAN_MSG_OBJ_TYPE_TX,
		.id_mask = 0,
		.flags = CAN_MSG_OBJ_NO_FLAGS,
		.len = 4
	},
	/*----------------*/
	{
		.obj_id = static_cast<uint32_t>(comm::CanTxMsgType::CURRENT_PHC) + 1,
		.id = 0x000000A6,
		.frame = CAN_MSG_FRAME_STD,
		.type = CAN_MSG_OBJ_TYPE_TX,
		.id_mask = 0,
		.flags = CAN_MSG_OBJ_NO_FLAGS,
		.len = 4
	},
	/*----------------*/
	{
		.obj_id = static_cast<uint32_t>(comm::CanTxMsgType::TEMPERATURE_PHA) + 1,
		.id = 0x000000A7,
		.frame = CAN_MSG_FRAME_STD,
		.type = CAN_MSG_OBJ_TYPE_TX,
		.id_mask = 0,
		.flags = CAN_MSG_OBJ_NO_FLAGS,
		.len = 4
	},
	/*----------------*/
	{
		.obj_id = static_cast<uint32_t>(comm::CanTxMsgType::TEMPERATURE_PHB) + 1,
		.id = 0x000000A8,
		.frame = CAN_MSG_FRAME_STD,
		.type = CAN_MSG_OBJ_TYPE_TX,
		.id_mask = 0,
		.flags = CAN_MSG_OBJ_NO_FLAGS,
		.len = 4
	},
	/*----------------*/
	{
		.obj_id = static_cast<uint32_t>(comm::CanTxMsgType::TEMPERATURE_PHC) + 1,
		.id = 0x000000A9,
		.frame = CAN_MSG_FRAME_STD,
		.type = CAN_MSG_OBJ_TYPE_TX,
		.id_mask = 0,
		.flags = CAN_MSG_OBJ_NO_FLAGS,
		.len = 4
	},
	/*----------------*/
	{
		.obj_id = static_cast<uint32_t>(comm::CanTxMsgType::TEMPERATURE_CASE) + 1,
		.id = 0x000000AA,
		.frame = CAN_MSG_FRAME_STD,
		.type = CAN_MSG_OBJ_TYPE_TX,
		.id_mask = 0,
		.flags = CAN_MSG_OBJ_NO_FLAGS,
		.len = 4
	},
	/*----------------*/
	{
		.obj_id = static_cast<uint32_t>(comm::CanTxMsgType::STATE) + 1,
		.id = 0x000000AB,
		.frame = CAN_MSG_FRAME_STD,
		.type = CAN_MSG_OBJ_TYPE_TX,
		.id_mask = 0,
		.flags = CAN_MSG_OBJ_NO_FLAGS,
		.len = 4
	},
	/*----------------*/
	{
		.obj_id = static_cast<uint32_t>(comm::CanTxMsgType::FAULT) + 1,
		.id = 0x000000AC,
		.frame = CAN_MSG_FRAME_STD,
		.type = CAN_MSG_OBJ_TYPE_TX,
		.id_mask = 0,
		.flags = CAN_MSG_OBJ_NO_FLAGS,
		.len = 4
	},
	/*----------------*/
	{
		.obj_id = static_cast<uint32_t>(comm::CanTxMsgType::CURRENT_D) + 1,
		.id = 0x000000AD,
		.frame = CAN_MSG_FRAME_STD,
		.type = CAN_MSG_OBJ_TYPE_TX,
		.id_mask = 0,
		.flags = CAN_MSG_OBJ_NO_FLAGS,
		.len = 4
	},
	/*----------------*/
	{
		.obj_id = static_cast<uint32_t>(comm::CanTxMsgType::CURRENT_Q) + 1,
		.id = 0x000000AE,
		.frame = CAN_MSG_FRAME_STD,
		.type = CAN_MSG_OBJ_TYPE_TX,
		.id_mask = 0,
		.flags = CAN_MSG_OBJ_NO_FLAGS,
		.len = 4
	},
	/*----------------*/
	{
		.obj_id = static_cast<uint32_t>(comm::CanTxMsgType::ANGLE) + 1,
		.id = 0x000000AF,
		.frame = CAN_MSG_FRAME_STD,
		.type = CAN_MSG_OBJ_TYPE_TX,
		.id_mask = 0,
		.flags = CAN_MSG_OBJ_NO_FLAGS,
		.len = 4
	},
	/*----------------*/
	{
		.obj_id = static_cast<uint32_t>(comm::CanTxMsgType::H_MAG) + 1,
		.id = 0x000000B0,
		.frame = CAN_MSG_FRAME_STD,
		.type = CAN_MSG_OBJ_TYPE_TX,
		.id_mask = 0,
		.flags = CAN_MSG_OBJ_NO_FLAGS,
		.len = 8
	},
	/*----------------*/
	{
		.obj_id = static_cast<uint32_t>(comm::CanTxMsgType::H_PHASE) + 1,
		.id = 0x000000B1,
		.frame = CAN_MSG_FRAME_STD,
		.type = CAN_MSG_OBJ_TYPE_TX,
		.id_mask = 0,
		.flags = CAN_MSG_OBJ_NO_FLAGS,
		.len = 8
	},
	/*----------------*/
};

mcu::CanMsg mcu::Can::rx_msg_[32] =
{
	/*----------------*/
	{
		.obj_id = 32 - static_cast<uint32_t>(comm::CanRxMsgType::RX),
		.id = 0x000001F0,
		.frame = CAN_MSG_FRAME_STD,
		.type = CAN_MSG_OBJ_TYPE_RX,
		.id_mask = 0,
		.flags = CAN_MSG_OBJ_RX_INT_ENABLE,
		.len = 4
	},
	/*----------------*/
	{
		.obj_id = 32 - static_cast<uint32_t>(comm::CanRxMsgType::SPEED),
		.id = 0x000001F1,
		.frame = CAN_MSG_FRAME_STD,
		.type = CAN_MSG_OBJ_TYPE_RX,
		.id_mask = 0,
		.flags = CAN_MSG_OBJ_RX_INT_ENABLE,
		.len = 4
	},
	/*----------------*/
	{
		.obj_id = 32 - static_cast<uint32_t>(comm::CanRxMsgType::ANGLE),
		.id = 0x000001F2,
		.frame = CAN_MSG_FRAME_STD,
		.type = CAN_MSG_OBJ_TYPE_RX,
		.id_mask = 0,
		.flags = CAN_MSG_OBJ_RX_INT_ENABLE,
		.len = 4
	}
	/*----------------*/
};



#endif /* SRC_COMM_CAN_CAN_COMM_MSGS_H_ */
