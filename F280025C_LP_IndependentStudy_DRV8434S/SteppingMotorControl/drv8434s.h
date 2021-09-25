/*
 * drv8434s.h
 *
 *  Created on: 2021/09/12
 *      Author: junpe
 */

#ifndef STEPPINGMOTORCONTROL_DRV8434S_H_
#define STEPPINGMOTORCONTROL_DRV8434S_H_

#include <stdint.h>
#include <stdlib.h>

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>

#include <ti/sysbios/knl/Task.h>

#include "device.h"
#include <syscfg/board.h>

// GPIO PIN
#define __DRV8434S_CS_PIN DRV8434S_CS
#define __DRV8434S_SLEEP_PIN DRV8434S_SLEEP
#define __DRV8434S_EN_PIN DRV8434S_EN
#define __DRV8434S_DIR_PIN DRV8434S_DIR

// Peripherals
#define __DRV8434S_SPI_BASE DRV8434S_SPI_BASE
#define __DRV8434S_CURRENT_DAC_EPWM_BASE DAC_EPWM_BASE
#define __DRV8434S_CONTROL_EPWM_BASE DRV8434S_EPWM_BASE

void DRV8434S_write(uint16_t addr, uint16_t data);
uint16_t DRV8434S_read(uint16_t addr);
void DRV8434S_setFullScaleCurrentLimit(float current);
void DRV8434S_setFrequency(uint32_t freq);
void DRV8434S_setMicroStepping(uint16_t div);
uint16_t DRV8434S_getMicroStepping();
void DRV8434S_init();


#endif /* STEPPINGMOTORCONTROL_DRV8434S_H_ */
