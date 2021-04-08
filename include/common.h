/*
 * common.h
 *
 *  Created on: 23 сент. 2020 г.
 *      Author: oleg
 */

#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

#include <motorcontrol/math.h>


namespace Phase {
enum Phase
{
	A,
	B,
	C
};
}

namespace Fault {
enum Fault
{
	NONE = 0x00,
	UNDERVOLTAGE = 0x01,
	OVERVOLTAGE = 0x02,
	OVERCURRENT = 0x04,
	DRIVER_FLT = 0x08,
	JUNCTION_OVERTEMP = 0x10,
	CASE_OVERTEMP = 0x20
};

inline void SetFault(uint32_t& fault_code, Fault fault)
{
	fault_code = fault_code | fault;
}

inline void ResetFault(uint32_t& fault_code, Fault fault)
{
	fault_code = fault_code ^ fault;
}

inline void ResetFault(uint32_t& fault_code)
{
	fault_code = 0;
}
}  // namespace Fault

/*
 * @brief
 */
template <typename T, uint32_t n>
class SimpleArray
{
private:
    T data_[n];
public:
	T& operator[] (uint32_t i) {return data_[i];}
	const T& operator[] (uint32_t i) const {return data_[i];}
	uint32_t size() const {return n;}
	const T* data() const {return data_;}
};


/**
 * @brief
 * @param
 * @return
 */
inline float ToRadiansPerSec(float speed, int pole_pairs)
{
	return 2 * MATH_PI * pole_pairs * speed / 60;
}

/**
 * @brief
 * @param
 * @return
 */
inline float ToRpm(float speed, int pole_pairs)
{
	return 60 * speed / (2 * MATH_PI * pole_pairs);
}

/**
 * @brief
 * @param
 * @return
 */
inline float ToRadian(float degree)
{
	return MATH_PI * degree / 180;
}

/**
 * @brief
 * @param
 * @return
 */
inline float ToDeg(float radian)
{
	return 180 * radian / MATH_PI;
}


#endif /* INCLUDE_COMMON_H_ */
