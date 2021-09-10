/*
 *  ======== main.c ========
 */

#include <xdc/std.h>

#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>

#include <ti/sysbios/knl/Task.h>

#include "device.h"
#include <syscfg/board.h>

/*
 *  ======== taskFxn ========
 */
Void taskFxn_Heartbeat(UArg a0, UArg a1)
{
    System_printf("enter taskFxn_Heartbeat()\n");

    while(1) {
      System_printf("LED ON\n");
      System_flush();
      GPIO_writePin(USER_LED_RED, 1);
      GPIO_writePin(USER_LED_GREEN, 1);
      Task_sleep(500);
      System_printf("LED OFF\n");
      System_flush();
      GPIO_writePin(USER_LED_RED, 0);
      GPIO_writePin(USER_LED_GREEN, 0);
      Task_sleep(500);
    }
}

void taskFxn_DRV8434S(UArg a0, UArg a1) {
  System_printf("enter taskFxn_DRV8434S()\n");
  System_flush();


  // Wake up DRV8434S
  GPIO_writePin(DRV8434S_CS, 1);
  GPIO_writePin(DRV8434S_SLEEP, 1);

  Task_sleep(100);

  // Fault Flag clear
  GPIO_writePin(DRV8434S_CS, 0);
  SPI_writeDataBlockingFIFO(DRV8434S_SPI_BASE, (uint16_t)0x0000 + (0x06 <<9) + 0xb0);

  uint16_t recv_data = 0;
  recv_data = SPI_readDataBlockingFIFO(DRV8434S_SPI_BASE);
  GPIO_writePin(DRV8434S_CS, 1);

//  System_printf("DRV8434S's 0x06 is %x\n", (recv_data & 0x00ff));
//  System_flush();

  // Read 0x08
  GPIO_writePin(DRV8434S_CS, 0);
  SPI_writeDataBlockingFIFO(DRV8434S_SPI_BASE, (uint16_t)0x4000 + (0x08 <<9));

  recv_data = SPI_readDataBlockingFIFO(DRV8434S_SPI_BASE);
  GPIO_writePin(DRV8434S_CS, 1);

//  System_printf("DRV8434S's 0x08 is %x\n", (recv_data & 0x00ff));
//  System_flush();

  EPWM_setTimeBaseCounterMode(DRV8434S_EPWM, EPWM_COUNTER_MODE_UP);
  EPWM_setTimeBasePeriod(DRV8434S_EPWM, )

  System_printf("exit taskFxn_DRV8434S()\n");
  System_flush();
}

/*
 *  ======== main ========
 */
Int main()
{ 
    Task_Handle task_heartbeat, task_drv8434s;
    Error_Block eb;

    System_printf("enter main()\n");

    Device_init();
    Device_initGPIO();
    Board_init();

//    EINT;
//    ERTM;

    Error_init(&eb);
    task_heartbeat = Task_create(taskFxn_Heartbeat, NULL, &eb);
    if (task_heartbeat == NULL) {
        System_printf("Task_create() failed!\n");
        BIOS_exit(0);
    }

    task_drv8434s = Task_create(taskFxn_DRV8434S, NULL, &eb);
    if (task_drv8434s == NULL) {
      System_printf("Task_create() failed!\n");
      BIOS_exit(0);
    }

    BIOS_start();    /* does not return */
    return(0);
}
