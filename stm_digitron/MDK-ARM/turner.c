#include "stm32f10x.h"                  // Device header
int16_t Encoder_Count;//定义一个带符号的变量

void Encoder_Init(void)
{
    //配置RCC
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    
    //配置GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;//使用到PB0和PB1口
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    
    //配置AFIO
    GPIO_EXTILineConfig(RCC_APB2Periph_GPIOB,GPIO_PinSource0);
    GPIO_EXTILineConfig(RCC_APB2Periph_GPIOB,GPIO_PinSource1); //AFIO每个中断是分开的
    
    //配置EXTI
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line=EXTI_Line1|EXTI_Line0;//使用到中断0和中断1
    EXTI_InitStructure.EXTI_LineCmd=ENABLE;
    EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling; //下降沿触发
    EXTI_Init(&EXTI_InitStructure);
    
    //配置NVIC，这里配置了俩个中断
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//调用一次即可，只能有一个，有多个必须保证相同
    NVIC_InitTypeDef NVIC_InitStructure;//结构体命名一次即可
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

