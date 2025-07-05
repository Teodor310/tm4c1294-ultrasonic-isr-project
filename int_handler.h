#include "inc/tm4c1294ncpdt.h"
#include <stdint.h>
#include <stdio.h>
#ifndef INT_HANDLER_H_
#define INT_HANDLER_H_

#define original    1
#define reverse     0


void IntPortJHandler(void);
void IntPortDHandler(void);
// void IntPortLHandler(void);
void config_gpio_interrupts(void);
void  TIMER1A_Handler(void);
volatile uint32_t duration;
volatile float startTime;
volatile float stopTime;

#define ULTRASONIC_TRIGGER 0x01  // PD0
#define ULTRASONIC_ECHO    0x02  // PD1


#endif
