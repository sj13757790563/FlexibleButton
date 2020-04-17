
#ifndef __RJ_LED_H
#define __RJ_LED_H

#include "main.h"

typedef struct 
{
	uint8_t ucEnalbe;//用于点亮或者熄灭
	uint8_t ucState;//状态变量，用于 点亮和熄灭的区分。
	uint16_t usBeepTime;//点亮时间，单位10ms
	uint16_t usStopTime;//熄灭时间，单位10ms
	uint16_t usCycle;//点亮和熄灭的循环次数
	uint16_t usCount;//用于点亮和熄灭时的计数。
	uint16_t usCycleCount;//用于循环次数计数。
	uint8_t ucMute;		/* 用于熄灭LED */	
}LED_T;


typedef enum
{
  RJ_LD1=0,
	RJ_LD2,
	RJ_LD3,
  RJ_LD_END
}RJ_LED_ID;




/*需要用户实现的函数 */
void LightUpLed(RJ_LED_ID _id);
void ExtinguishedLed(RJ_LED_ID _id);

/* 供外部调用的函数声明 */
void LED_Init(void);
void LED_Start(RJ_LED_ID _id,uint16_t _usBeepTime, uint16_t _usStopTime, uint16_t _usCycle);
void LED_Stop(RJ_LED_ID _id);


/*需要放入循环的函数*/
void LED_Pro_ISR_10MS(void);



#endif



