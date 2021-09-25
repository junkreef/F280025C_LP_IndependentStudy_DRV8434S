/*
 * Heartbeat_tasks.c
 *
 *  Created on: 2021/09/15
 *      Author: a0489849
 */


#include "Heartbeat_tasks.h"

Void taskFxn_Heartbeat(UArg a0, UArg a1) {
  System_printf("enter taskFxn_Heartbeat()\n");

  while (1) {
//      System_printf("LED ON\n");
//      System_flush();
    GPIO_writePin(USER_LED_GREEN, 1);
    Task_sleep(500);
//      System_printf("LED OFF\n");
//      System_flush();
    GPIO_writePin(USER_LED_GREEN, 0);
    Task_sleep(500);
  }
}
