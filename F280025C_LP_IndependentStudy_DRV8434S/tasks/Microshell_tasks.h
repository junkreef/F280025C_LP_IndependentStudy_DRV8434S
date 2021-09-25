/*
 * Microshell_tasks.h
 *
 *  Created on: 2021/09/15
 *      Author: a0489849
 */

#ifndef TASKS_MICROSHELL_TASKS_H_
#define TASKS_MICROSHELL_TASKS_H_

#include <string.h>
#include <stdio.h>

#include <xdc/std.h>

#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/family/c28/Hwi.h>

#include "../device/device.h"
#include <syscfg/board.h>

#include "../microshell/core/microshell.h"
#include "../microshell/util/mscmd.h"
#include "../microshell/core/msconf.h"

#include "../SteppingMotorControl/motion_control.h"

#include "DRV8434S_tasks.h"


Void taskFxn_Microshell(UArg a0, UArg a1);

#endif /* TASKS_MICROSHELL_TASKS_H_ */
