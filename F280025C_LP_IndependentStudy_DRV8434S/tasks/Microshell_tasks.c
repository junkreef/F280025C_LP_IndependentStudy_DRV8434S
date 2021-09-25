/*
 * Microshell_tasks.c
 *
 *  Created on: 2021/09/15
 *      Author: a0489849
 */

#include "Microshell_tasks.h"

static MSCMD_USER_RESULT usrcmd_hello(MSOPT *msopt, MSCMD_USER_OBJECT usrobj);
static MSCMD_USER_RESULT usrcmd_init(MSOPT *msopt, MSCMD_USER_OBJECT usrobj);
static MSCMD_USER_RESULT usrcmd_speed(MSOPT *msopt, MSCMD_USER_OBJECT usrobj);
static MSCMD_USER_RESULT usrcmd_pps(MSOPT *msopt, MSCMD_USER_OBJECT usrobj);

static MICROSHELL ms;
static MSCMD mscmd;

static Semaphore_Handle semaphore_sci_rx, semaphore_sci_tx;
static Hwi_Handle hwi_sci_rx, hwi_sci_tx;

Task_Params task_params;
Hwi_Params hwi_param;
Semaphore_Params semaphore_param;
Error_Block eb;

static MSCMD_COMMAND_TABLE table[] = {
    {"hello", usrcmd_hello},
    {"init", usrcmd_init},
    {"speed", usrcmd_speed},
};

const char __welcome_message[] =
    "\033[2J\033[fC2000 + DRV8434S Evaluation Program\n\r";
char msg_buffer[64] = {0};

void hwiFxn_SCI_RX(UArg arg) {
  DINT;
  SCI_disableInterrupt(XDS_SCI_BASE, SCI_INT_RXFF);
  EINT;
  Semaphore_post(semaphore_sci_rx);
}

void hwiFxn_SCI_TX(UArg arg) {
  DINT;
  SCI_disableInterrupt(XDS_SCI_BASE, SCI_INT_TXFF);
  EINT;
  Semaphore_post(semaphore_sci_tx);
}

static char urx(void) {
  SCI_enableInterrupt(XDS_SCI_BASE, SCI_INT_RXFF);
  Semaphore_pend(semaphore_sci_rx, BIOS_WAIT_FOREVER);
  char c = SCI_readCharNonBlocking(XDS_SCI_BASE);
  SCI_clearInterruptStatus(XDS_SCI_BASE, SCI_INT_RXFF);
  return c;
}

static void utx(char c) {
  SCI_enableInterrupt(XDS_SCI_BASE, SCI_INT_TXFF);
  Semaphore_pend(semaphore_sci_tx, BIOS_WAIT_FOREVER);
  SCI_clearInterruptStatus(XDS_SCI_BASE, SCI_INT_TXFF);
  SCI_writeCharNonBlocking(XDS_SCI_BASE, c);


//  SCI_writeCharBlockingFIFO(XDS_SCI_BASE, c);
}

static void utx_array(const uint16_t * const array, uint16_t length) {
  for(uint16_t cnt=0; cnt < length; cnt++) {
    utx(array[cnt]);
  }
}

static void terminal_puts(const char* const array) {
  utx_array((uint16_t*)array, strlen(array));
}

static void action_hook(MSCORE_ACTION action) {

}

Void taskFxn_Microshell(UArg a0, UArg a1) {

  char buf[MSCONF_MAX_INPUT_LENGTH];
  char PS1[] = "> ";
  MSCMD_USER_RESULT r;

  microshell_init(&ms, utx, urx, action_hook);
  mscmd_init(&mscmd, table, sizeof(table) / sizeof(table[0]), NULL);

  Error_init(&eb);

  Semaphore_Params_init(&semaphore_param);
  semaphore_param.mode = Semaphore_Mode_BINARY;
  semaphore_sci_rx = Semaphore_create(0, &semaphore_param, &eb);

  Semaphore_Params_init(&semaphore_param);
  semaphore_param.mode = Semaphore_Mode_BINARY;
  semaphore_sci_tx = Semaphore_create(0, &semaphore_param, &eb);

  Hwi_Params_init(&hwi_param);
  hwi_sci_rx = Hwi_create(96, hwiFxn_SCI_RX, &hwi_param, &eb);
  Hwi_Params_init(&hwi_param);
  hwi_sci_tx = Hwi_create(97, hwiFxn_SCI_TX, &hwi_param, &eb);

  SCI_setFIFOInterruptLevel(XDS_SCI_BASE, SCI_FIFO_TX0, SCI_FIFO_RX1);

  terminal_puts(__welcome_message);

  while(1) {
    terminal_puts(PS1);
    microshell_getline(&ms, buf, sizeof(buf));
    mscmd_execute(&mscmd, buf, &r);
  }
}

static MSCMD_USER_RESULT usrcmd_hello(MSOPT *msopt, MSCMD_USER_OBJECT usrobj) {
  terminal_puts("Hello microshell world!\n\r");

  return 0;
}

static MSCMD_USER_RESULT usrcmd_init(MSOPT *msopt, MSCMD_USER_OBJECT usrobj) {
  Task_Params_init(&task_params);
  task_params.stackSize = 512;
  task_params.priority = 2;
  if (Task_create(taskFxn_DRV8434S, &task_params, &eb) == NULL) {
    System_printf("Task_create() failed!\n");
    BIOS_exit(0);
  }
  return 0;
}

static MSCMD_USER_RESULT usrcmd_speed(MSOPT *msopt, MSCMD_USER_OBJECT usrobj) {
  static char buf[MSCONF_MAX_INPUT_LENGTH];
  int argc;
  const char help_str[] =
      "PPS Command Usage:\n\r"
      "  pps COMMAND\n\r"
      "\n\r"
      "COMMAND:\n\r"
      "  set TARGET_PPS\n\r"
      "    set new pulse per seconds\n\r"
      "  show\n\r"
      "    display current pulse per seconds\n\r"
      "  help\n\r"
      "    display this message\n\r"
      "\n\r";

  msopt_get_argc(msopt, &argc);

  if (argc == 1) {
    terminal_puts(help_str);
    return 0;
  }

  if (argc == 2) {
    msopt_get_argv(msopt, 1, buf, sizeof(buf));
    if (strcmp("show", buf) != 0){
      terminal_puts(help_str);
      return 0;
    }
    sprintf(msg_buffer, "%lf\n\r", motion_control_get_speed_target());
    terminal_puts(msg_buffer);
    return 0;
  }

  if (argc == 3) {
    msopt_get_argv(msopt, 1, buf, sizeof(buf));
    if (strcmp("set", buf) != 0){
      terminal_puts(help_str);
      return 0;
    }
    msopt_get_argv(msopt, 2, buf, sizeof(buf));
    motion_control_set_speed_target(strtof(buf, NULL));
    return 0;
  }

  return -1;
}

static MSCMD_USER_RESULT usrcmd_pps(MSOPT *msopt, MSCMD_USER_OBJECT usrobj) {
  static char buf[MSCONF_MAX_INPUT_LENGTH];
  int argc;
  const char help_str[] =
      "PPS Command Usage:\n\r"
      "  pps COMMAND\n\r"
      "\n\r"
      "COMMAND:\n\r"
      "  set TARGET_PPS\n\r"
      "    set new pulse per seconds\n\r"
      "  show\n\r"
      "    display current pulse per seconds\n\r"
      "  help\n\r"
      "    display this message\n\r"
      "\n\r";

  msopt_get_argc(msopt, &argc);

  if (argc == 1) {
    terminal_puts(help_str);
    return 0;
  }

  if (argc == 2) {
    msopt_get_argv(msopt, 1, buf, sizeof(buf));
    if (strcmp("show", buf) != 0){
      terminal_puts(help_str);
      return 0;
    }
    sprintf(msg_buffer, "%ld\n\r", motion_control_get_pps_target());
    terminal_puts(msg_buffer);
    return 0;
  }

  if (argc == 3) {
    msopt_get_argv(msopt, 1, buf, sizeof(buf));
    if (strcmp("set", buf) != 0){
      terminal_puts(help_str);
      return 0;
    }
    msopt_get_argv(msopt, 2, buf, sizeof(buf));
    motion_control_set_pps_target(strtoul(buf, NULL, 10));
    return 0;
  }

  return -1;
}
