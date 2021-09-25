/*
 * motion_control.c
 *
 *  Created on: 2021/09/21
 *      Author: a0489849
 */


#include "motion_control.h"

float (*ramping_profile)(uint32_t transition_time_ms, float* target);

// Pulse per seconds
// pps_target[0] = previous target
// pps_target[1] = next target
uint32_t pps_target[2] = {0};

float speed_target[2] = {0};

void motion_control_set_speed_target(float speed){
  speed_target[1] = speed;
}

float motion_control_get_speed_target(){
  return speed_target[1];
}

void motion_control_set_pps_target(uint32_t pps){
  pps_target[1] = pps;
}

uint32_t motion_control_get_pps_target(){
  return pps_target[1];
}

void motion_control_set_ramping_profile(float (*ramping_profile_function)(uint32_t transition_time_ms, float* target)) {
  ramping_profile = ramping_profile_function;
}

void motion_control_update_speed() {
  static uint32_t transition_time = 0;
  static float speed_current = 0;

  if (speed_target[1] == speed_current) {
    // No need to update
    return;
  }

  if(speed_target[1] == 0){
    DRV8434S_setFrequency(0);
    return;
  }

  float speed_new = (*ramping_profile)(transition_time, speed_target);

  if(speed_target[0] < speed_target[1]) {
    // accelerate situation
    if(speed_target[1] < speed_new) {
      // prevent over speed_target
      speed_new = speed_target[1];
      speed_target[0] = speed_target[1];
    }
  } else {
    // decelerate situation
    if(speed_target[1] > speed_new) {
        // prevent under speed_target
      speed_new = speed_target[1];
        speed_target[0] = speed_target[1];
    }
  }

  if (-2 < speed_new || speed_new < 2) {
    DRV8434S_setMicroStepping(256);
  } else if (-5 < speed_new || speed_new < 5) {
    DRV8434S_setMicroStepping(64);
  } else {
    DRV8434S_setMicroStepping(16);
  }

  if(speed_new < 0) {
    speed_new = -speed_new;
    GPIO_writePin(__DRV8434S_DIR_PIN, 1);
  }
  if(speed_new > 0) {
    GPIO_writePin(__DRV8434S_DIR_PIN, 0);
  }

  DRV8434S_setFrequency(200.0 * DRV8434S_getMicroStepping() * (speed_new / 60));

  speed_current = speed_new;

  return;
}

//void motion_control_update_pps(){
//  static uint32_t transition_time = 0;
//  static uint32_t pps_current = 0;
//
//  if(pps_target[1] == pps_current) {
//    // No need to update
//    return;
//  }
//
//  // Need to update pps
//  uint32_t pps_new = (*ramping_profile)(transition_time, pps_target);
//
//  if(pps_target[0] < pps_target[1]) {
//    // accelerate situation
//    if(pps_target[1] < pps_new) {
//      // prevent over pps_target
//      pps_new = pps_target[1];
//      pps_target[0] = pps_target[1];
//    }
//  } else {
//    // decelerate situation
//    if(pps_target[1] > pps_new) {
//        // prevent under pps_target
//        pps_new = pps_target[1];
//        pps_target[0] = pps_target[1];
//    }
//  }
//
//  DRV8434S_setFrequency(pps_new);
//  pps_current = pps_new;
//
//}

float motion_control_ramping_profile_nope(uint32_t transition_time_ms, float* target){
  return target[1];
}
