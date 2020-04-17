#ifndef __SIMPLE_DEVICE_H
#define __SIMPLE_DEVICE_H

#include "main.h"

/*
   该程序用来驱动一些比较简单的器件,只需要运行即可的器件
*/

typedef struct 
{
	uint8_t ucEnalbe;//用于使能或者失能
	uint8_t ucState;//状态变量，用于 运行和 停止的区分。
	uint16_t usBeepTime;//运行时间，单位1ms
	uint16_t usStopTime;//停止时间，单位1ms
	uint16_t usCycle;//点亮和熄灭的循环次数
	uint16_t usCount;//用于点亮和熄灭时的计数。
	uint16_t usCycleCount;//用于循环次数计数。
	uint8_t ucMute;		/* 用于熄灭LED */	
}SIMPLE_DEVICE_T;


typedef enum
{
    RJ_dev0=0,//可以是LED,LD1
		RJ_dev1,//可以是风扇,LD2
		RJ_dev2,//可以是蜂鸣器LD3
    RJ_dev_END
}RJ_DEVICE_ID;




/*需要用户实现的函数 */
void simple_devece_enable(RJ_DEVICE_ID _id);
void simple_devece_disable(RJ_DEVICE_ID _id);

/* 供外部调用的函数声明 */
void simple_devece_Init(void);
void simple_devece_Start(RJ_DEVICE_ID _id,uint16_t _usBeepTime, uint16_t _usStopTime, uint16_t _usCycle);
void simple_devece_Stop(RJ_DEVICE_ID _id);


/*需要放入循环的函数*/
void simple_device_ISR_1MS(void);



#endif