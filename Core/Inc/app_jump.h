#ifndef APP_JUMP_H_
#define APP_JUMP_H_

#include "stm32f4xx_hal.h"

#define DVC_APPJMP_CURRENT_ADDRESS 0x8000000

void dvc_appjmp_init_vectors(void);
void dvc_appjmp_goto(uint32_t);
void dvc_appjmp_enable(void);

#endif
