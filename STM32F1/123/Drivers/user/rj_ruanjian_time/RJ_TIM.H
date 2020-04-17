


#ifndef __RJ_TIM_H
#define __RJ_TIM_H


#include "main.h"

typedef enum 
{
	RJ_TIM0 = 0,
	RJ_TIM1 = 1,
	RJ_TIM2,	
	RJ_TIM3,
	RJ_TIM4,
	RJ_TIM_END,
}TMR_ID_E;

typedef enum
{
TMR_ONCE_MODE = 0, /* 一次工作模式 */
TMR_AUTO_MODE = 1 /* 自动定时工作模式 */
}TMR_MODE_E;






/* 定时器结构体，成员变量必须增加__IO 即 volatile，因为这个变量在中断和主程序中同时被访问，
有可能造成编译器错误优化。
*/
typedef struct
{
volatile TMR_MODE_E Mode; /* 计数器模式， 1 次性 */
volatile uint8_t Flag; /* 定时到达标志 */
volatile uint32_t Count; /* 计数器 */
volatile uint32_t PreLoad; /* 计数器预装值 */
}SOFT_TMR;






void _RJ_TIM_1MS_ISR(void);
void _InitTimer(void);
uint32_t _GetRunTime(void);
void _StartTimer(TMR_ID_E _id, uint32_t _period);
void _StartAutoTimer(TMR_ID_E _id, uint32_t _period);
void bsp_StopTimer(TMR_ID_E _id);
uint8_t _CheckTimer(TMR_ID_E _id);












#endif















