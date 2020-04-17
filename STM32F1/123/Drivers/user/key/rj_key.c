#include "rj_key.h"

RJ_KEY_T g_tkey[RJ_KEY_END]; /* 定义key全局结构体变量 */
static KEY_FIFO_T s_tKey[RJ_KEY_END];		/* 按键FIFO变量,结构体 */
KEY_event_FIFO_T user_key_event;//按键事件结构体

void bsp_PutKey(RJ_KEY_ID _id,uint8_t _KeyCode);
uint8_t bsp_GetKey(RJ_KEY_ID _id);
uint8_t bsp_GetKey2(RJ_KEY_ID _id);


void rj_key_error(void)
{
    uint8_t i=1;
    while (i)
    {
        /* code */
    }
}


/*
*********************************************************************************************************
*	函 数 名: get_key
*	功能说明: 获取按键当前状态
*	形    参: _id :LED灯ID
*	返 回 值: 0:没被按下,1:按下
*********************************************************************************************************
*/
uint8_t  get_key(RJ_KEY_ID _id)
{
    uint8_t ack=0;
	
	if(_id == RJ_KEY1)
	{
		if( HAL_GPIO_ReadPin(SW1_GPIO_Port,SW1_Pin) == 0) ack=1;
		else ack=0;	
	}
	else if(_id == RJ_KEY2)
	{
		if( HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin) == 0) ack=1;
		else ack=0;	
	}
	else if(_id == RJ_KEY3)
	{	
		if( HAL_GPIO_ReadPin(SW3_GPIO_Port,SW3_Pin) == 0) ack=1;
		else ack=0;
	}
    return ack;
}	


uint8_t key_event[KEY_FIFO_SIZE]={0};
uint32_t user_time_key=0;
void key_loop_1ms(void)
{
		user_time_key++;
    RJ_KEY_ID i = 0;
    for ( i = 0; i < RJ_KEY_END; i++)
    {
        /* code */
        
        if(get_key(i))
        {
            //如果是按下
            g_tkey[i].enableTime++;
        }
        else
        {
            //松开
            g_tkey[i].disableTime++;            
        }
        
        //周期+1
         g_tkey[i].usCycle++;


         if(g_tkey[i].usCycle >= KEY_CYC)
         {
             if(g_tkey[i].enableTime == 0)
             {
                 //只检测到 松开
                 g_tkey[i].now=0;
             }
             else if (g_tkey[i].disableTime == 0)
             {
                 //只检测到按下
                 g_tkey[i].now=1;
             }
             else
             {
                    ;//其他情况重新检测

             }

            //清除标志位
            g_tkey[i].enableTime=0;
            g_tkey[i].disableTime =0;
            g_tkey[i].usCycle =0;

            if( g_tkey[i].now != g_tkey[i].before)
            {
                //当前和上一个状态不相同
								user_time_key=0;	//开启时间
                if(g_tkey[i].now == 0)
                {
                    //songkai
                    bsp_PutKey(i,2*i+2);
                }
                else
                {
                    //anxia
                    bsp_PutKey(i,2*i+1);                    
                }                
                //xiugai 上一个状态
                g_tkey[i].before=g_tkey[i].now ;
            }
            else
            {
                ;//如果相同则结束
            }
         }
    }
		
		uint8_t j=0;
		if(user_time_key >= key_event_time_duan)
		{
      user_time_key=0;	      
			for ( i = 0; i < RJ_KEY_END; i++)
            {
                //检测所有按键

                for (j = 0; j< KEY_FIFO_SIZE; j++)
                {
                    /* code */
                    key_event[j]=bsp_GetKey(i);

                    if(key_event[j] == 0)
                    {                        
                        break;
                    }
                }

                if(j == 1)
                {
                    if (key_event[0] == (2*i+1))
                    {
                        //最后一个状态是按下

                    	if(g_tkey[i].ChangCounter >= key_event_time_chang)
                    	{
                    		//按下时间大于预设值
                    		bsp_putEvent(i,EVENT_CHANGAN);
                    		g_tkey[i].ChangCounter=0;

                    	}
                    	else
                    	{
                    		g_tkey[i].ChangCounter++;
                    		bsp_PutKey(i,2*i+1);
                    	}
                    }
                    else
                    {
                        ;
                    }
                }
                else if (j==2)
                {
                   if (key_event[1] == (2*i+1))
                   {
                        //最后一个状态是按下
                        bsp_PutKey(i,2*i+1); 
                        g_tkey[i].ChangCounter++;
                   }
                   else
                   {
                       /* code */
										 
										  if(g_tkey[i].ChangCounter >= key_event_time_chang)
											{
												bsp_putEvent(i,EVENT_CHANGAN);
												g_tkey[i].ChangCounter=0;
											}
											else
											{
												bsp_putEvent(i,EVENT_DANJI);
												g_tkey[i].ChangCounter=0;												
											}                      
                   }
                }
                else if ( j== 4)
                {
                    if (key_event[1] == (2*i+1))
                    {   
                        //最后一个状态是按下
                        ;
                    }
                    else
                    {
                        bsp_putEvent(i,EVENT_SHUANGJI);
                    }                    
                } 
            }
											
		}		
		
}


/*
*********************************************************************************************************
*	函 数 名: bsp_PutKey
*	功能说明: 将1个键值压入按键FIFO缓冲区。可用于模拟一个按键。
*	形    参:  _KeyCode : 按键代码
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_PutKey(RJ_KEY_ID _id,uint8_t _KeyCode)
{
	s_tKey[_id].Buf[s_tKey[_id].Write] = _KeyCode;

	if (++s_tKey[_id].Write  >= KEY_FIFO_SIZE)
	{
		s_tKey[_id].Write = 0;
	}
}


/*
*********************************************************************************************************
*	函 数 名: bsp_GetKey
*	功能说明: 从按键FIFO缓冲区读取一个键值。
*	形    参: 无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
uint8_t bsp_GetKey(RJ_KEY_ID _id)
{
	uint8_t ret;

	if (s_tKey[_id].Read == s_tKey[_id].Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_tKey[_id].Buf[s_tKey[_id].Read];

		if (++s_tKey[_id].Read >= KEY_FIFO_SIZE)
		{
			s_tKey[_id].Read = 0;
		}
		return ret;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_GetKey2
*	功能说明: 从按键FIFO缓冲区读取一个键值。独立的读指针。
*	形    参:  无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
uint8_t bsp_GetKey2(RJ_KEY_ID _id)
{
	uint8_t ret;

	if (s_tKey[_id].Read2 == s_tKey[_id].Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_tKey[_id].Buf[s_tKey[_id].Read2];

		if (++s_tKey[_id].Read2 >= KEY_FIFO_SIZE)
		{
			s_tKey[_id].Read2 = 0;
		}
		return ret;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitKeyVar
*	功能说明: 初始化按键变量
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitKeyVar(void)
{
	uint8_t i;



	/* 给每个按键结构体成员变量赋一组缺省值 */
    for (uint8_t i = 0; i < RJ_KEY_END; i++)
    {
        /* code */
				/* 对按键FIFO读写指针清零 */
			s_tKey[i].Read = 0;
			s_tKey[i].Write = 0;
			s_tKey[i].Read2 = 0;
			
      g_tkey[i].now=0;
      g_tkey[i].before=0;
      g_tkey[i].usCycle=0;
    }
}


/*
*********************************************************************************************************
*	函 数 名: bsp_putEvent
*	功能说明: 将1个事件压入事件FIFO缓冲区。可用于模拟一个按键。
*	形    参:  _id      : 按键ID
               event    :按键事件
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_putEvent(RJ_KEY_ID _id,KEY_EVENT_ENUM event)
{
	// s_tKey[_id].Buf[s_tKey[_id].Write] = _KeyCode;

	// if (++s_tKey[_id].Write  >= KEY_FIFO_SIZE)
	// {
	// 	s_tKey[_id].Write = 0;
	// }
    
  user_key_event.Buf[user_key_event.Write]._id=_id;
	user_key_event.Buf[user_key_event.Write].event=event;	

    if (++user_key_event.Write  >= KEY_FIFO_SIZE)
		{
			user_key_event.Write = 0;
		}
}


/*
*********************************************************************************************************
*	函 数 名: bsp_GetEvent
*	功能说明: 从事件FIFO缓冲区读取一个键值。
*	形    参: 无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
RJ_KEY_event_T bsp_GetEvent(void)
{
	RJ_KEY_event_T ret;

	if (user_key_event.Read == user_key_event.Write)
	{
		ret._id=0;
    ret.event=EVENT_NONE;
	}
	else
	{
		ret = user_key_event.Buf[user_key_event.Read];

		if (++user_key_event.Read >= KEY_FIFO_SIZE)
		{
			user_key_event.Read = 0;
		}		
	}
	return ret;
}

















