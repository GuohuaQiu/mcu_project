//=============================================================================
//文件名称：main.h
//功能概要：STM32F103C8核心检测
//版权所有：源地工作室www.vcc-gnd.com
//版权更新：2013-02-20
//调试方式：J-Link OB ARM SW方式 5MHz
//=============================================================================

//头文件
#include "stm32f10x.h"
#include "GPIOLIKE51.h"

//函数声明
void GPIO_Configuration(void);

//=============================================================================
//文件名称：Delay
//功能概要：延时
//参数说明：nCount：延时长短
//函数返回：无
//=============================================================================

void Delay(uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

int16_t Number;


int16_t Encoder_Count;//定义一个带符号的变量

void Encoder_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;//结构体命名一次即可
    //配置RCC
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    
    //配置GPIO
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;//使用到PB0和PB1口
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    
    //配置AFIO
    GPIO_EXTILineConfig(RCC_APB2Periph_GPIOB,GPIO_PinSource0);
    GPIO_EXTILineConfig(RCC_APB2Periph_GPIOB,GPIO_PinSource1); //AFIO每个中断是分开的
    
    //配置EXTI
    EXTI_InitStructure.EXTI_Line=EXTI_Line1|EXTI_Line0;//使用到中断0和中断1
    EXTI_InitStructure.EXTI_LineCmd=ENABLE;
    EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling; //下降沿触发
    EXTI_Init(&EXTI_InitStructure);
    
    //配置NVIC，这里配置了俩个中断
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//调用一次即可，只能有一个，有多个必须保证相同
    NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn;//中断0的配置
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级为1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;//响应优先级为1
    
    NVIC_InitStructure.NVIC_IRQChannel=EXTI1_IRQn;//中断1的配置
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=2 ;//响应优先级为2，即中断1的优先级略低于中断0的优先级
    NVIC_Init(&NVIC_InitStructure);//NVIC初始化

    
}

int16_t Return_Encoder_Count(void) //返回Encoder_Count的值，如果正转就返回+1，反转就返回-1。方便共主函数操作
{
    int16_t Temp;
    Temp=Encoder_Count;              
    Encoder_Count=0;
    return Temp;
    
}

//中断0函数
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetFlagStatus(EXTI_Line0)==SET)
    {
        EXTI_ClearITPendingBit(EXTI_Line0);//清除中断标志位，以便下一次进入中断
        if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==1)  //如果正转
        {
            Encoder_Count++;  
        }
        
    }
    
}
//中断1函数
void EXTI1_IRQHandler(void)
{
    if(EXTI_GetFlagStatus(EXTI_Line1)==SET)
    {
        EXTI_ClearITPendingBit(EXTI_Line1);
        if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==1)  //如果反转
        {
            Encoder_Count--;
        }
    }
    
}


//=============================================================================
//文件名称：main
//功能概要：主函数
//参数说明：无
//函数返回：int
//=============================================================================
int main(void)
{
    int i;
	int16_t x;
    int m = 0;
	GPIO_Configuration();
	//Encoder_Init();
	
    // while (1)
	// 	{
    x = 0;

    while (1)
    {
        // Below code works.
        // for (i = 0; i < 7; i++)
        // {
        //     //   if (x < 10000)
        //     // PAout(i) = 1;
        //     OUT_DIGIT(x%16);
        // }
        // SetPos(x%4);
        // Delay(10000000);
        // OUT_DIGIT(1000);
        // Delay(1000000);


        x++;

        a_show();
        a_work();
        // ShowDigit(1);
        // b_finding();
    }
}

//=============================================================================
//文件名称：GPIO_Configuration
//功能概要：GPIO初始化
//参数说明：无
//函数返回：无
//=============================================================================
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE); 						 
//=============================================================================
//LED -> PC13
//=============================================================================			 
  GPIO_InitStructure.GPIO_Pin = 0x1e0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);


  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE); 						 
//=============================================================================
//LED -> PC13
//=============================================================================			 
  GPIO_InitStructure.GPIO_Pin = 0x7f;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}



