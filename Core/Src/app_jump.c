#include <string.h>
#include "app_jump.h"
#include "stm32f4xx_hal.h" // Include the proper header here :)

uint32_t g_pfnVectors[256];

static uint32_t vectorTable_RAM[256] __attribute__(( aligned(0x200ul) ));
static uint8_t enabled=0;


void dvc_appjmp_init_vectors()
{
	__disable_irq();
	memmove(vectorTable_RAM,g_pfnVectors,256*sizeof(uint32_t));
	SCB->VTOR = 0x8000000;
	__DSB();
	__ISB();
}

void dvc_appjmp_goto(uint32_t app_addr)
{
	if(enabled != 1)
	   return;
	__disable_irq();
	for(int i = 0;i < 8;i++) NVIC->ICER[i] = 0xFFFFFFFF;
	for(int i = 0;i < 8;i++) NVIC->ICPR[i] = 0xFFFFFFFF;
	__set_CONTROL(0);
	__set_MSP(*(__IO uint32_t*)app_addr);
	uint32_t JumpAddress = *((volatile uint32_t*) (app_addr + 4));
	__ISB();
	__DSB();
	SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);
	void (*reset_handler)(void) = (void*)JumpAddress;
	while(1)
	   reset_handler();
}

void dvc_appjmp_enable()
{
	enabled = 1;
}
