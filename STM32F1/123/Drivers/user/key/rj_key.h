#ifndef __RJ_KEY_H
#define __RJ_KEY_H

#include "main.h"

//按键采样周期
#define  KEY_CYC 8
//按键事件归类时间,在300个按键循环周期内发生的按键/按下+松开被视为1次单击事件
#define key_event_time_duan 	500
#define key_event_time_chang 	2		//表示5个按键归类时间内,按键都是按下的,触发一次长按事件

typedef struct 
{
	uint8_t now;//当前状态 0:松开 1按下
	uint8_t before;//上一个状态 0松开 1按下
	uint16_t enableTime;//按下计数器，单位1ms
	uint16_t disableTime;//松开时间，单位1ms
	uint16_t usCycle;//周期计数器 单位1ms
	uint32_t ChangCounter;//长按计数器

}RJ_KEY_T;



typedef enum
{
  RJ_KEY1=0,
  RJ_KEY2,
  RJ_KEY3,
  RJ_KEY_END
}RJ_KEY_ID;


/* 按键FIFO用到变量 */
#define KEY_FIFO_SIZE	10
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* 键值缓冲区 */
	uint8_t Read;					/* 缓冲区读指针1 */
	uint8_t Write;					/* 缓冲区写指针 */
	uint8_t Read2;					/* 缓冲区读指针2 */
}KEY_FIFO_T;

/*
	定义键值代码, 必须按如下次序定时每个键的按下、弹起和长按事件

	推荐使用enum, 不用#define，原因：
	(1) 便于新增键值,方便调整顺序，使代码看起来舒服点
	(2) 编译器可帮我们避免键值重复。
*/
typedef enum
{
	KEY_NONE = 0,			/* 0 表示按键事件 */

	KEY_1_DOWN,				/* 1键按下 */
	KEY_1_UP,				/* 1键弹起 */	

	KEY_2_DOWN,				/* 2键按下 */
	KEY_2_UP,				/* 2键弹起 */	

	KEY_3_DOWN,				/* 3键按下 */
	KEY_3_UP,				/* 3键弹起 */	

}KEY_ENUM;



typedef enum
{
	EVENT_NONE = 0,			/* 0 表示按键事件 */
	EVENT_DANJI,				/* 单击 */
	EVENT_SHUANGJI,				/* 双击 */	
	EVENT_CHANGAN,				/* 长按 */	

}KEY_EVENT_ENUM;

typedef struct 
{
	RJ_KEY_ID _id;//按键ID,
	KEY_EVENT_ENUM event;//按键事件

}RJ_KEY_event_T;

/* 事件FIFO用到变量 */
typedef struct
{
	RJ_KEY_event_T Buf[KEY_FIFO_SIZE];		/* 键值缓冲区 */
	uint8_t Read;					/* 缓冲区读指针1 */
	uint8_t Write;					/* 缓冲区写指针 */
	uint8_t Read2;					/* 缓冲区读指针2 */
}KEY_event_FIFO_T;


//需要实现的函数
uint8_t  get_key(RJ_KEY_ID _id);


//供外部调用

void bsp_putEvent(RJ_KEY_ID _id,KEY_EVENT_ENUM event);
RJ_KEY_event_T bsp_GetEvent(void);

//初始化
void bsp_InitKeyVar(void);

//周期调用
void key_loop_1ms(void);


#endif