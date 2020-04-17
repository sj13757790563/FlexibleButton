#ifndef __RJ_PWM_H
#define __RJ_PWM_H

#include "main.h"


//PWM ID
typedef enum
{
    RJ_PWM1=0,	//PE13
		RJ_PWM2,		//PE14
    RJ_PWM3,
    RJ_PWM_END
}RJ_PWM_ID;


typedef struct 
{
	uint8_t ucEnalbe;//用于启动或者关闭	
    uint32_t count; //记录循环的次数
    uint32_t count_reg; //记录循环的最大次数	
    uint32_t h_coun;    //高电平持续的次数	
}RJ_PWM_T;

//需要外部实现的函数

void pwm_out_io_h(RJ_PWM_ID _id);
void pwm_out_io_l(RJ_PWM_ID _id);


//外部调用的函数

void rj_pwm_init(void);
void rj_pwm_time_loop(void);
void rj_pwm_code_loop(void);
void rj_pwm_qidong(RJ_PWM_ID _id);
void rj_pwm_guanbi(RJ_PWM_ID _id,uint8_t sta);
void set_rj_pwm(RJ_PWM_ID _id,uint32_t cyc,uint32_t h_cyc);




#endif


