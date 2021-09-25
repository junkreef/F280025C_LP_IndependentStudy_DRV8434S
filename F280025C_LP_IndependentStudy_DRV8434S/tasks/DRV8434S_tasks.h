/*
 * DRV8434S_tasks.h
 *
 *  Created on: 2021/09/15
 *      Author: a0489849
 */

#ifndef TASKS_DRV8434S_TASKS_H_
#define TASKS_DRV8434S_TASKS_H_

#include <xdc/std.h>

#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>

#include "../device/device.h"
#include <syscfg/board.h>
#include "../SteppingMotorControl/drv8434s.h"
#include "../SteppingMotorControl/motion_control.h"

void clockFxn_UpdateFreq(UArg a);
void taskFxn_DRV8434S(UArg a0, UArg a1);

#endif /* TASKS_DRV8434S_TASKS_H_ */
