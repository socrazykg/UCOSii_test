/* Includes ------------------------------------------------------------------*/
#include <ucos_ii.h>
#include "stm32f10x.h"
#include "led.h"

static OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE];
static OS_STK task1_stk[TASK1_STK_SIZE];
static OS_STK task2_stk[TASK2_STK_SIZE];

static void systick_init(void){
	RCC_ClocksTypeDef rcc_clocks;
	RCC_GetClocksFreq(&rcc_clocks);
	SysTick_Config(rcc_clocks.HCLK_Frequency / OS_TICKS_PER_SEC);
}

static void task1(void *p_arg){
	for (;;){
		led_on(LED_0);
		OSTimeDly(500);
		led_off(LED_0);
		OSTimeDly(500);		
	}
}

static void task2(void *p_arg){
	for (;;){
		led_on(LED_1);
		OSTimeDly(700);
		led_off(LED_1);
		OSTimeDly(700);
	}
}


static void startup_task(void *p_arg){
	INT8U err;
	systick_init();     /* Initialize the SysTick. */

#if (OS_TASK_STAT_EN > 0)
	OSStatInit();      /* Determine CPU capacity. */
#endif
 /* TODO: create application tasks here */
	err = OSTaskCreate(task1, (void *)0, 
						&task1_stk[TASK1_STK_SIZE - 1], TASK1_PRIO);
	err = OSTaskCreate(task2, (void *)0, 
						&task2_stk[TASK2_STK_SIZE - 1], TASK2_PRIO);
	if(OS_ERR_NONE != err)
		while(1)
			;
	OSTaskDel(OS_PRIO_SELF);
}




int main(void){
	led_init();
	OSInit();
	OSTaskCreate(startup_task, (void *)0,
              &startup_task_stk[STARTUP_TASK_STK_SIZE - 1],
              STARTUP_TASK_PRIO);
	OSStart();
	return 0;
}

