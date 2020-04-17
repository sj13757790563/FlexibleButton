
#include "simple_device.h"



SIMPLE_DEVICE_T g_tLED[RJ_dev_END]; /* 定义驱动全局结构体变量 */



void rj_simple_devece_error(void)
{
    uint8_t i=1;
    while (i)
    {
        /* code */
    }
}

/*
*********************************************************************************************************
*	函 数 名: simple_devece_enable
*	功能说明: 运行器件
*	形    参: _id :器件ID
*	返 回 值: 无
*********************************************************************************************************
*/
__weak void simple_devece_enable(RJ_DEVICE_ID _id)
{
    //rj_simple_devece_error();
	
	if(_id == RJ_dev0)
	{
		HAL_GPIO_WritePin(LD1_GPIO_Port,LD1_Pin,GPIO_PIN_RESET);
	
	}
	else if(_id == RJ_dev1)
	{
		HAL_GPIO_WritePin(LD2_GPIO_Port,LD2_Pin,GPIO_PIN_RESET);
	}
	else if(_id == RJ_dev2)
	{
		HAL_GPIO_WritePin(LD3_GPIO_Port,LD3_Pin,GPIO_PIN_RESET);
	}
	
	
	
}


/*
*********************************************************************************************************
*	函 数 名: simple_devece_disable
*	功能说明: 停止器件
*	形    参: _id :器件ID
*	返 回 值: 无
*********************************************************************************************************
*/
__weak void simple_devece_disable(RJ_DEVICE_ID _id)
{
    //rj_simple_devece_error();
	if(_id == RJ_dev0)
	{
		HAL_GPIO_WritePin(LD1_GPIO_Port,LD1_Pin,GPIO_PIN_SET);
	
	}
	else if(_id == RJ_dev1)
	{
		HAL_GPIO_WritePin(LD2_GPIO_Port,LD2_Pin,GPIO_PIN_SET);
	}
	else if(_id == RJ_dev2)
	{
		HAL_GPIO_WritePin(LD3_GPIO_Port,LD3_Pin,GPIO_PIN_SET);
	}
}

/*
*********************************************************************************************************
*	函 数 名: LED_Init
*	功能说明: 初始化LED
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void simple_devece_Init(void)
{   
    uint8_t i=0;
    for(i=0;i<RJ_dev_END;i++)
    {
        g_tLED[i].ucMute=0;
    }	
}


/*
*********************************************************************************************************
*	函 数 名: LED_Start
*	功能说明: 启动LED。
*	形    参:   _id:器件ID
                _usBeepTime : 使能时间，单位1ms; 0 表示熄灭
*			  _usStopTime : 熄灭时间，单位1ms; 0 表示持续点亮
*			  _usCycle : 循环次数， 0 表示持续
*	返 回 值: 无
*********************************************************************************************************
*/
void simple_devece_Start(RJ_DEVICE_ID _id,uint16_t _usBeepTime, uint16_t _usStopTime, uint16_t _usCycle)
{    
    
    if (_usBeepTime == 0 || g_tLED[_id].ucMute == 1)
	{
		return;
	}
	g_tLED[_id].usBeepTime = _usBeepTime;
	g_tLED[_id].usStopTime = _usStopTime;
	g_tLED[_id].usCycle = _usCycle;
	g_tLED[_id].usCount = 0;
	g_tLED[_id].usCycleCount = 0;
	g_tLED[_id].ucState = 0;
	g_tLED[_id].ucEnalbe = 1;	/* 设置完全局参数后再使能点亮标志 */

	simple_devece_enable(_id);			/* 开始点亮 */
}

/*
*********************************************************************************************************
*	函 数 名: LED_Stop
*	功能说明: 熄灭。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void simple_devece_Stop(RJ_DEVICE_ID _id)
{
	g_tLED[_id].ucEnalbe = 0;

	if ((g_tLED[_id].usStopTime == 0) || (g_tLED[_id].usCycle == 0))
	{
		simple_devece_disable(_id);	/* 必须在清控制标志后再熄灭，避免停止后在中断中又开启 */
	}
}


/*
*********************************************************************************************************
*	函 数 名: simple_device_1MS
*	功能说明: 每隔1ms调用1次该函数，用于控制器件。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void simple_device_ISR_1MS(void)
{
	uint8_t i=0;

    for( i=0;i<RJ_dev_END;i++)
    {
        if ((g_tLED[i].ucEnalbe == 0) || (g_tLED[i].usStopTime == 0) || (g_tLED[i].ucMute == 1))
        {
            continue;
        }

        if (g_tLED[i].ucState == 0)
        {
            if (g_tLED[i].usStopTime > 0)	/* 间断 */
            {
                if (++g_tLED[i].usCount >= g_tLED[i].usBeepTime)
                {
                    simple_devece_disable(i);		/* 停止 */
                    g_tLED[i].usCount = 0;
                    g_tLED[i].ucState = 1;
                }
            }
            else
            {
                ;	/* 不做任何处理，连续 */
            }
        }
        else if (g_tLED[i].ucState == 1)
        {
            if (++g_tLED[i].usCount >= g_tLED[i].usStopTime)
            {
                /* 连续时，直到调用stop停止为止 */
                if (g_tLED[i].usCycle > 0)
                {
                    if (++g_tLED[i].usCycleCount >= g_tLED[i].usCycle)
                    {
                        /* 循环次数到，停止 */
                        g_tLED[i].ucEnalbe = 0;
                    }

                    if (g_tLED[i].ucEnalbe == 0)
                    {
                        g_tLED[i].usStopTime = 0;
                        continue;
                    }
                }

                g_tLED[i].usCount = 0;
                g_tLED[i].ucState = 0;

                simple_devece_enable(i);			/* 开始 */
            }
        }
    }
}