/*
 * vector_calc.h
 *
 *  Created on: 20 окт. 2020 г.
 *      Author: Oleg Aushev
 */

#ifndef INCLUDE_VECTOR_MATH_H_
#define INCLUDE_VECTOR_MATH_H_

#include <math.h>
#include "motorcontrol/math.h"
#include "motorcontrol/clarke.h"
#include "motorcontrol/park.h"
#include "motorcontrol/ipark.h"
#include <utility>
#include "common.h"

namespace vectormath
{
/**
 * @brief
 * @param
 * @return
 */
inline std::pair<float, float> ParkTransform(float alpha, float beta, float sine, float cosine)
{
	PARK park_struct =
	{
		.Alpha = alpha,
		.Beta = beta,
		.Sine = sine,
		.Cosine = cosine
	};
	runPark(&park_struct);
	return std::make_pair(park_struct.Ds, park_struct.Qs);
}


/**
 * @brief
 * @param
 * @return
 */
inline std::pair<float, float> InvParkTransform(float d, float q, float sine, float cosine)
{
	IPARK ipark_struct =
	{
		.Ds = d,
		.Qs = q,
		.Sine = sine,
		.Cosine = cosine
	};
	runIPark(&ipark_struct);
	return std::make_pair(ipark_struct.Alpha, ipark_struct.Beta);
}


/**
 * @brief
 * @param
 * @return
 */
inline std::pair<float, float> ClarkeTransform(float a, float b, float c)
{
	CLARKE clarke_struct =
	{
		.As = a,
		.Bs = b,
		.Cs = c
	};
	runClarke1(&clarke_struct);
	return std::make_pair(clarke_struct.Alpha, clarke_struct.Beta);
}


/**
 * @brief
 */
struct PwmCompensation
{
	float dt;			// Мертвое время
	float uvt;			// Падение напряжения на транзисторе
	float uvd;			// Падение напряжения на диоде
};


/**
 * @brief
 * @param
 * @param
 * @param
 * @return
 */
template <typename T>
inline void CalculateSvPwm(float angle,
						   float magnitude,
						   float dc_voltage,
						   uint16_t pwm_counter_period,
						   SimpleArray<T, 3>& pwm_cmp_values)
{
	int32_t sector = (int32_t)(angle / ToRadian(60));
	float beta = angle - (float)sector * ToRadian(60);

	//base vector times calculating
	float tb1 = sqrtf(3) * (magnitude / dc_voltage) * sinf(ToRadian(60) - beta);
	float tb2 = sqrtf(3) * (magnitude / dc_voltage) * sinf(beta);
	float tb0 = (1.f - tb1 - tb2) / 2.f;

	SimpleArray<float, 3> pulse_times;
	switch (sector)
	{
	case 0:
		pulse_times[0] = tb1 + tb2 + tb0;
		pulse_times[1] = tb2 + tb0;
		pulse_times[2] = tb0;
		break;
	case 1:
		pulse_times[0] = tb1 + tb0;
		pulse_times[1] = tb1 + tb2 + tb0;
		pulse_times[2] = tb0;
		break;
	case 2:
		pulse_times[0] = tb0;
		pulse_times[1] = tb1 + tb2 + tb0;
		pulse_times[2] = tb2 + tb0;
		break;
	case 3:
		pulse_times[0] = tb0;
		pulse_times[1] = tb1 + tb0;
		pulse_times[2] = tb1 + tb2 + tb0;
		break;
	case 4:
		pulse_times[0] = tb2 + tb0;
		pulse_times[1] = tb0;
		pulse_times[2] = tb1 + tb2 + tb0;
		break;
	case 5:
		pulse_times[0] = tb1 + tb2 + tb0;
		pulse_times[1] = tb0;
		pulse_times[2] = tb1 + tb0;
		break;
	default:
		break;
	}

	for(uint32_t i = 0; i < 3; i++)
	{
		if (pulse_times[i] < 0.f)
		{
			pulse_times[i] = 0.f;
		}
		else if (pulse_times[i] > 1.f)
		{
			pulse_times[i] = 1.f;
		}
		pwm_cmp_values[i] = (T)(pulse_times[i] * pwm_counter_period);
	}
}


/**
 * @brief
 * @param
 * @param
 * @param
 * @return
 *//*
void CompensatePwm(const ArrayN<float, 3>& phase_currents)
{
	float uznam __attribute__((unused));
	uznam = pwm_compensation.udc - pwm_compensation.uvt + pwm_compensation.uvd;
	float dt2 = pwm_compensation.dt;

	if(phase_currents.data[PHASE_A] > 0){
		pulse_times.data[0] += dt2;
	}else{
		pulse_times.data[0] -= dt2;
	}
	if(phase_currents.data[PHASE_B] > 0){
		pulse_times.data[1] += dt2;
	}else{
		pulse_times.data[1] -= dt2;
	}
	if(phase_currents.data[PHASE_C] > 0){
		pulse_times.data[2] += dt2;
	}else{
		pulse_times.data[2] -= dt2;
	}
	if(pulse_times.data[0] < 0.f){
		switch_times.data[0] = 0.f;
	}else {
		if(pulse_times.data[0] > 1.0f){
			switch_times.data[0] = 1.0f;
		}
	}
	if(pulse_times.data[1] < 0.f){
		pulse_times.data[1] = 0.f;
	}else {
		if(pulse_times.data[1] > 1.0f){
			pulse_times.data[1] = 1.0f;
		}
	}
	if(pulse_times.data[2] < 0.f){
		pulse_times.data[2] = 0.f;
	}else {
		if(pulse_times.data[2] > 1.0f){
			pulse_times.data[2] = 1.0f;
		}
	}
	for(int i = 0; i < 3; i++){
		switch_times.data[i] = (uint32_t)(pulse_times.data[i]*pwm_counter_period_);
	}
}
*/

} /* namespace VectorCalc */

#endif /* INCLUDE_VECTOR_MATH_H_ */
