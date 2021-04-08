/*
 * can_mcu.h
 *
 *  Created on: 2 дек. 2020 г.
 *      Author: Oleg Aushev
 */

#ifndef SRC_MCU_CAN_CAN_MCU_H_
#define SRC_MCU_CAN_CAN_MCU_H_

#include "driverlib.h"
#include "device.h"
#include <common.h>

void ErrorHandler(uint32_t error_code);

namespace mcu {

enum CanModule
{
	CANA,
	CANB
};

enum CanBitrate
{
	CAN_BITRATE_125K,
	CAN_BITRATE_500K,
	CAN_BITRATE_1M,
};

struct CanMsg
{
	const uint32_t obj_id;
	const uint32_t id;
	const CAN_MsgFrameType frame;
	const CAN_MsgObjType type;
	const uint32_t id_mask;
	const uint32_t flags;
	const uint16_t len;
	uint16_t data[8];
};

class Can
{
private:
	static const uint32_t reg_bases_[2];
	static const uint32_t tx_pins_[2];
	static const uint32_t tx_pin_configs_[2];
	static const uint32_t rx_pins_[2];
	static const uint32_t rx_pin_configs_[2];
	const uint32_t reg_base_;

	static const size_t tx_msg_count_;
	static const size_t rx_msg_count_;
	static CanMsg tx_msg_[32];
	static CanMsg rx_msg_[32];

public:
	Can(CanModule can, CanBitrate bitrate);

#ifdef CPU1
	static void TransferControlToCpu2(CanModule can);
#endif

	template <typename T, uint32_t data_size>
	void SendArray(size_t msg_type, const SimpleArray<T, data_size>& data);

	template <typename T>
	void Send(size_t msg_type, T data);

	bool Receive(size_t msg_type);

	template<typename T, uint32_t data_size>
	void GetDataToArray(size_t msg_type, SimpleArray<T, data_size>& data);

	template<typename T>
	T GetData(size_t msg_type);
};

/* ---------------- Template methods definitions ---------------- */
/*
 * @brief
 * @param
 * @return
 */
template <typename T, uint32_t data_size>
void Can::SendArray(size_t msg_type, const SimpleArray<T, data_size>& data)
{
	switch (sizeof(T)) 	/* WARNING! C28 byte is 16-bit long */
	{
	case 1:
		if (tx_msg_[msg_type].len != data_size * 2)
		{
			ErrorHandler(__LINE__);
			return;
		}
		for (int i = 0; i < data_size; ++i)
		{
			uint16_t buff;				/* sizeof(uint32_t, float) == 2 */
			memcpy(&buff, &data[i], 1);	/* 0x12345678 stores in memory as 0x5678-0x1234 */
			tx_msg_[msg_type].data[2*i] = (buff >> 8) & 0x00FF;
			tx_msg_[msg_type].data[2*i+1]   = buff & 0x00FF;
		}
		break;

	case 2:
		if (tx_msg_[msg_type].len != data_size * 4)
		{
			ErrorHandler(__LINE__);
			return;
		}
		for (int i = 0; i < data_size; ++i)
		{
			uint16_t buff_long[2];		/* sizeof(uint32_t, float) == 2 */
			memcpy(&buff_long, &data[i], 2);	/* 0x12345678 stores in memory as 0x5678-0x1234 */
			tx_msg_[msg_type].data[4*i] = (buff_long[1] >> 8) & 0x00FF;
			tx_msg_[msg_type].data[4*i+1] = buff_long[1] & 0x00FF;
			tx_msg_[msg_type].data[4*i+2] = (buff_long[0] >> 8) & 0x00FF;
			tx_msg_[msg_type].data[4*i+3]   = buff_long[0] & 0x00FF;
		}
		break;

	default:
		return;
	}

	CAN_sendMessage(reg_base_,
					tx_msg_[msg_type].obj_id,
					tx_msg_[msg_type].len,
					tx_msg_[msg_type].data);
}

/*
 * @brief
 * @param
 * @return
 */
template <typename T>
void Can::Send(size_t msg_type, T data)
{
	switch (sizeof (T))
	{
	case 1:
		if (tx_msg_[msg_type].len != 2)
		{
			ErrorHandler(__LINE__);
			return;
		}
		uint16_t buff;
		memcpy(&buff, &data, 1);
		tx_msg_[msg_type].data[0] = (buff >> 8) & 0x00FF;
		tx_msg_[msg_type].data[1]   = buff & 0x00FF;
		break;

	case 2:
		if (tx_msg_[msg_type].len != 4)
		{
			ErrorHandler(__LINE__);
			return;
		}

		uint16_t buff_long[2];			/* sizeof(uint32_t, float) == 2 */
		memcpy(&buff_long, &data, 2);	/* 0x12345678 stores in memory as 0x5678-0x1234 */
		tx_msg_[msg_type].data[0] = (buff_long[1] >> 8) & 0x00FF;
		tx_msg_[msg_type].data[1] = buff_long[1] & 0x00FF;
		tx_msg_[msg_type].data[2] = (buff_long[0] >> 8) & 0x00FF;
		tx_msg_[msg_type].data[3]   = buff_long[0] & 0x00FF;
		break;

	default:
		return;
	}

	CAN_sendMessage(reg_base_,
					tx_msg_[msg_type].obj_id,
					tx_msg_[msg_type].len,
					tx_msg_[msg_type].data);
}

/*
 * @brief
 * @param
 * @return
 */
template<typename T, uint32_t data_size>
void Can::GetDataToArray(size_t msg_type, SimpleArray<T, data_size>& data)
{
	switch (sizeof(T))
	{
	case 1:
		if (rx_msg_[msg_type].len != data_size * 2)
		{
			ErrorHandler(__LINE__);
			return;
		}
		for (int i = 0; i < data_size; ++i)
		{
			data[i] = rx_msg_[msg_type].data[2*i] << 8 | rx_msg_[msg_type].data[2*i+1];
		}
		break;

	case 2:
		if (rx_msg_[msg_type].len != data_size * 4)
		{
			ErrorHandler(__LINE__);
			return;
		}
		for (int i = 0; i < data_size; ++i)
		{
			uint16_t buff[2];	/* sizeof(uint32_t, float) == 2 */
			buff[1] = rx_msg_[msg_type].data[4*i] | rx_msg_[msg_type].data[4*i+1] << 8;
			buff[0] = rx_msg_[msg_type].data[4*i+2] | rx_msg_[msg_type].data[4*i+3] << 8;
			memcpy(&data[i], &buff, 2);
		}
		break;

	default:
		return;
	}
}

/*
 * @brief
 * @param
 * @return
 */
template<typename T>
T Can::GetData(size_t msg_type)
{
	T result;

	switch (sizeof(T))
	{
	case 1:
		if (rx_msg_[msg_type].len != 2)
		{
			ErrorHandler(__LINE__);
		}
		uint16_t buff;
		buff = rx_msg_[msg_type].data[0] << 8 | rx_msg_[msg_type].data[1];
		memcpy(&result, &buff, 1);
		break;

	case 2:
		if (rx_msg_[msg_type].len != 4)
		{
			ErrorHandler(__LINE__);
		}
		uint16_t buff_long[2];	/* sizeof(uint32_t, float) == 2 */
		buff_long[1] = rx_msg_[msg_type].data[0] << 8 | rx_msg_[msg_type].data[1];
		buff_long[0] = rx_msg_[msg_type].data[2] << 8 | rx_msg_[msg_type].data[3];
		memcpy(&result, &buff_long, 2);
		break;

	default:
		break;
	}
	return result;
}

} /* namespace mcu */


#endif /* SRC_MCU_CAN_CAN_MCU_H_ */
