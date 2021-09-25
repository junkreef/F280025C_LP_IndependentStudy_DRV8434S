/*
 *  ======== main.c ========
 */

#include <xdc/std.h>

#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>

#include "device/device.h"
#include <syscfg/board.h>
#include "SteppingMotorControl/drv8434s.h"

#include "tasks/Heartbeat_tasks.h"
#include "tasks/DRV8434S_tasks.h"
#include "tasks/Microshell_tasks.h"

/*
 *  ======== main ========
 */
Int main() {
  Task_Handle task_heartbeat, task_drv8434s, task_microshell;
  Task_Params task_params;
  Error_Block eb;

  System_printf("enter main()\n");

  Device_init();
  Device_initGPIO();
  Board_init();

  Error_init(&eb);
  Task_Params_init(&task_params);
  task_params.priority = 1;
  task_heartbeat = Task_create(taskFxn_Heartbeat, NULL, &eb);
  if (task_heartbeat == NULL) {
    System_printf("Task_create() failed!\n");
    BIOS_exit(0);
  }

  task_params.priority = 1;
  task_params.stackSize = 2048;
    task_microshell = Task_create(taskFxn_Microshell, &task_params, &eb);
    if (task_microshell == NULL) {
      System_printf("Task_create() failed!\n");
      BIOS_exit(0);
    }

  BIOS_start(); /* does not return */
  return (0);
}
