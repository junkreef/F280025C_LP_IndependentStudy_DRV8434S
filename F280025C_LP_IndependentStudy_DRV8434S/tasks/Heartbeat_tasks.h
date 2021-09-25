/*
 * Heartbeat_tasks.h
 *
 *  Created on: 2021/09/15
 *      Author: a0489849
 */

#ifndef TASKS_HEARTBEAT_TASKS_H_
#define TASKS_HEARTBEAT_TASKS_H_

#include <xdc/std.h>

#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>

#include "../device/device.h"
#include <syscfg/board.h>

Void taskFxn_Heartbeat(UArg a0, UArg a1);

#endif /* TASKS_HEARTBEAT_TASKS_H_ */
