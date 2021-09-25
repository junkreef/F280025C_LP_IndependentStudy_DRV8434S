/*
 * motion_control.h
 *
 *  Created on: 2021/09/21
 *      Author: a0489849
 */

#ifndef STEPPINGMOTORCONTROL_MOTION_CONTROL_H_
#define STEPPINGMOTORCONTROL_MOTION_CONTROL_H_

#include "drv8434s.h"

void motion_control_set_pps_target(uint32_t pps);
uint32_t motion_control_get_pps_target();
void motion_control_set_speed_target(float rpm);
float motion_control_get_speed_target();
void motion_control_set_ramping_profile(float (*ramping_profile_function)(uint32_t transition_time_ms, float* target));
void motion_control_set_speed(float rpm);
float motion_control_get_speed(void);
void motion_control_update_speed();
void motion_control_update_pps();
float motion_control_ramping_profile_nope(uint32_t transition_time_ms, float* target);


#endif /* STEPPINGMOTORCONTROL_MOTION_CONTROL_H_ */
