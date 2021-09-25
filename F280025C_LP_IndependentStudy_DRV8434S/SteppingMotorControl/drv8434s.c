/*
 * drv8434s.c
 *
 *  Created on: 2021/09/12
 *      Author: junpe
 */


#include "drv8434s.h"

uint16_t current_microstep = 1;

void DRV8434S_write(uint16_t addr, uint16_t data) {
    GPIO_writePin(__DRV8434S_CS_PIN, 0);
    SPI_writeDataBlockingFIFO(__DRV8434S_SPI_BASE,(addr <<9) + data);
    SPI_readDataBlockingFIFO(__DRV8434S_SPI_BASE);
    GPIO_writePin(__DRV8434S_CS_PIN, 1);
}

uint16_t DRV8434S_read(uint16_t addr) {
    uint16_t ret_val;
    GPIO_writePin(__DRV8434S_CS_PIN, 0);
    SPI_writeDataBlockingFIFO(__DRV8434S_SPI_BASE, 0x4000 + (addr <<9));
    ret_val = SPI_readDataBlockingFIFO(__DRV8434S_SPI_BASE);
    GPIO_writePin(__DRV8434S_CS_PIN, 1);
    return ret_val;
}

void DRV8434S_setFullScaleCurrentLimit(float current) {
    ASSERT(0 > current && current < 2.5);

    EPWM_setCounterCompareValue(__DRV8434S_CURRENT_DAC_EPWM_BASE, EPWM_COUNTER_COMPARE_A, 40*current); // 100/2.5*current
}

void DRV8434S_setFrequency(uint32_t freq) {
    if(freq == 0) {
        EPWM_setActionQualifierAction(__DRV8434S_CONTROL_EPWM_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);
        return;
    }else{
        EPWM_setTimeBasePeriod(__DRV8434S_CONTROL_EPWM_BASE, 5000000/freq); // 10MHz / 2 / freq. = target_period
        EPWM_setActionQualifierAction(__DRV8434S_CONTROL_EPWM_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_TOGGLE, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);
        return;
    }

}

void DRV8434S_setMicroStepping(uint16_t div) {
  uint16_t reg = 0;
  switch(div) {
    case 1:
      reg = 0x00;
      break;

    case 2:
      reg = 0x03;
      break;

    case 4:
      reg = 0x04;
      break;

    case 8:
      reg = 0x05;
      break;

    case 16:
      reg = 0x06;
      break;

    case 32:
      reg = 0x07;
      break;

    case 64:
      reg = 0x08;
      break;

    case 128:
      reg = 0x09;
      break;

    case 256:
      reg = 0x0A;
      break;

    default:
      return;
  }
  current_microstep = div;
  DRV8434S_write(0x05, reg); // 1/256 step
}

uint16_t DRV8434S_getMicroStepping() {
  return current_microstep;
}

void DRV8434S_init() {
    // Wake up DRV8434S
    GPIO_writePin(__DRV8434S_CS_PIN, 1);
    GPIO_writePin(__DRV8434S_SLEEP_PIN, 1);

    Task_sleep(100);

    // Fault Flag clear
    DRV8434S_write(0x06, 0xb0);

    // set micro stepping
    DRV8434S_setMicroStepping(256);

    // set DECAY mode
    DRV8434S_write(0x04, 0x08 + 0x06);

    // Set Full scale current limit
    EPWM_setTimeBaseCounterMode(__DRV8434S_CURRENT_DAC_EPWM_BASE, EPWM_COUNTER_MODE_UP);
    EPWM_setClockPrescaler(__DRV8434S_CURRENT_DAC_EPWM_BASE, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_10); // EPWM Counter Clock is 10MHz
    EPWM_setTimeBasePeriod(__DRV8434S_CURRENT_DAC_EPWM_BASE, 100); // set Initial pwm clock to 100KHz
    EPWM_setCounterCompareShadowLoadMode(__DRV8434S_CURRENT_DAC_EPWM_BASE, EPWM_COUNTER_COMPARE_A, EPWM_COMP_LOAD_ON_CNTR_ZERO);
    EPWM_setCounterCompareValue(__DRV8434S_CURRENT_DAC_EPWM_BASE, EPWM_COUNTER_COMPARE_A, 0);
    EPWM_setActionQualifierAction(__DRV8434S_CURRENT_DAC_EPWM_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(__DRV8434S_CURRENT_DAC_EPWM_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    DRV8434S_setFullScaleCurrentLimit(1.5);

    // Pulse PWM module Init
    EPWM_setTimeBaseCounterMode(__DRV8434S_CONTROL_EPWM_BASE, EPWM_COUNTER_MODE_UP);
    EPWM_setClockPrescaler(__DRV8434S_CONTROL_EPWM_BASE, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_10); // EPWM Counter Clock is 10MHz

    // set EN
    DRV8434S_write(0x04, 0xc0);
    GPIO_writePin(__DRV8434S_EN_PIN, 1);
}
