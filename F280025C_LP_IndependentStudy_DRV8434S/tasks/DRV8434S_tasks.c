/*
 * DRV8434S_tasks.c
 *
 *  Created on: 2021/09/15
 *      Author: a0489849
 */

#include "DRV8434S_tasks.h"

void clockFxn_UpdateFreq(UArg a) {
  static uint32_t current_freq = 10000;
  static uint16_t up_down = 0;

  if (!up_down) {
    if(current_freq <= 100000){
      current_freq += 1000;
    }else{
      current_freq += 100;
    }
    motion_control_set_pps_target(current_freq);
    if (current_freq == 420000) {
      up_down = 1;
      GPIO_writePin(USER_LED_RED, 1);
    }
  } else {
    if (current_freq <= 100000) {
      current_freq -= 1000;
    } else {
      current_freq -= 100;
    }

    motion_control_set_pps_target(current_freq);
    if (current_freq == 10000) {
      up_down = 0;
      GPIO_writePin(USER_LED_RED, 0);
    }

  }
  return;
}

void taskFxn_DRV8434S(UArg a0, UArg a1) {
  DRV8434S_init();

  Clock_Params clock_params;
  Clock_Handle clock_update_freq, clock_update_pps;
  Error_Block eb;

  Error_init(&eb);
  Clock_Params_init(&clock_params);

  motion_control_set_ramping_profile(motion_control_ramping_profile_nope);

  clock_params.period = 1;
  clock_params.startFlag = TRUE;
//  clock_update_freq = Clock_create(clockFxn_UpdateFreq, 10, &clock_params, &eb);
//  clock_update_freq = Clock_create(motion_control_update_pps, 10, &clock_params, &eb);
//  if (clock_update_freq == NULL) {
//    System_abort("Clock create failed");
//  }

  clock_update_pps = Clock_create(motion_control_update_speed, 10, &clock_params, &eb);
  if (clock_update_pps == NULL) {
    System_abort("Clock create failed");
  }

//  Clock_start(clock_update_freq);
  Clock_start(clock_update_pps);

  Task_exit();
}
