
#include "RJ_LED.H"


LED_T g_tLED[RJ_LD_END]; /* 定义LED全局结构体变量 */



void rj_led_error(void)
{
    uint8_t i=1;
    while (i)
    {
        /* code */
    }
}

/*
*********************************************************************************************************
*	函 数 名: LightUpLed
*	功能说明: 点亮LED灯
*	形    参: _id :LED灯ID
*	返 回 值: 无
*********************************************************************************************************
*/
__weak void LightUpLed(RJ_LED_ID _id)
{
    rj_led_error();
}


/*
*********************************************************************************************************
*	函 数 名: ExtinguishedLed
*	功能说明: 熄灭LED灯
*	形    参: _id :LED灯ID
*	返 回 值: 无
*********************************************************************************************************
*/
__weak void ExtinguishedLed(RJ_LED_ID _id)
{
    rj_led_error();
}

/*
*********************************************************************************************************
*	函 数 名: LED_Init
*	功能说明: 初始化LED
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void LED_Init(void)
{   
    uint8_t i=0;
    for(i=0;i<RJ_LD_END;i++)
    {
        g_tLED[i].ucMute=0;
    }	
}


/*
*********************************************************************************************************
*	函 数 名: LED_Start
*	功能说明: 启动LED。
*	形    参:   _id:led
                _usBeepTime : 点亮时间，单位10ms; 0 表示熄灭
*			  _usStopTime : 熄灭时间，单位10ms; 0 表示持续点亮
*			  _usCycle : 循环次数， 0 表示持续
*	返 回 值: 无
*********************************************************************************************************
*/
void LED_Start(RJ_LED_ID _id,uint16_t _usBeepTime, uint16_t _usStopTime, uint16_t _usCycle)
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

	LightUpLed(_id);			/* 开始点亮 */
}

/*
*********************************************************************************************************
*	函 数 名: LED_Stop
*	功能说明: 熄灭。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void LED_Stop(RJ_LED_ID _id)
{
	g_tLED[_id].ucEnalbe = 0;

	if ((g_tLED[_id].usStopTime == 0) || (g_tLED[_id].usCycle == 0))
	{
		ExtinguishedLed(_id);	/* 必须在清控制标志后再熄灭，避免停止后在中断中又开启 */
	}
}


/*
*********************************************************************************************************
*	函 数 名: LED_Pro_ISR_10MS
*	功能说明: 每隔10ms调用1次该函数，用于控制LED。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void LED_Pro_ISR_10MS(void)
{
	uint8_t i=0;

    for( i=0;i<RJ_LD_END;i++)
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
                    ExtinguishedLed(i);		/* 停止 */
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

                LightUpLed(i);			/* 开始 */
            }
        }
    }
}









