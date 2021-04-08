/*
 * settings.h
 * MUST be included only in main.c
 *
 *  Created on: 3 дек. 2020 г.
 *      Author: oleg
 */

#ifndef SRC_SETTINGS_H_
#define SRC_SETTINGS_H_

#include "motor_drive/motor_drive.h"


namespace settings {

const motor::model::MotorParameters motor_params =
{
	.use_defaults = true,
	.pole_pairs = 4,
	.R 		= 0.025,
	.Psi 	= 0.0575, // Ifield = 16A
	.Ld 	= 260e-6,
	.k_Ld	= 0,
	.Lq 	= 120e-6,
	.k_Lq	= 0,
	.i_max	= 200,
	.start =
	{
		.didt				= 100,
		.acceleration 		= 30,
		.speed_max			= 2*3.14159*4*g_rpm_ref/60,
		.i_max 				= 200,
	}
};

const motor::model::Settings model_settings =
{
	.kp_start = 10,
	.ki_start = 2.5,
	.kp_speed	= 0.8,
	.ki_speed	= 0.0005,
	.kp_id	= 2.2,
	.ki_id = 0.022,
	.kp_iq = 1.1,
	.ki_iq = 0.011,
	.speed_filter_length = 64,
	.mras_settings =
	{
		.kp = 0.05,
		.ki = 1250
	}
};

const motor::Settings control_settings =
{
	.dc_uv_bound = -10,
	.dc_ov_bound = 300,
	.phase_oc_bound = 400
};

const xm3::Settings xm3_settings =
{
	.fund_freq = 300,
	.sw_freq = 10000,
	.dt = 100,
	.junction_ot_bound = 273 + 125,
	.case_ot_bound = 273 + 80
};

} /* namespace set */


#endif /* SRC_SETTINGS_H_ */
