#include "stm32f10x.h"                  // Device header
int16_t Encoder_Count;//����һ�������ŵı���

void Encoder_Init(void)
{
    //����RCC
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    
    //����GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;//ʹ�õ�PB0��PB1��
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    
    //����AFIO
    GPIO_EXTILineConfig(RCC_APB2Periph_GPIOB,GPIO_PinSource0);
    GPIO_EXTILineConfig(RCC_APB2Periph_GPIOB,GPIO_PinSource1); //AFIOÿ���ж��Ƿֿ���
    
    //����EXTI
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line=EXTI_Line1|EXTI_Line0;//ʹ�õ��ж�0���ж�1
    EXTI_InitStructure.EXTI_LineCmd=ENABLE;
    EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling; //�½��ش���
    EXTI_Init(&EXTI_InitStructure);
    
    //����NVIC�����������������ж�
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����һ�μ��ɣ�ֻ����һ�����ж�����뱣֤��ͬ
    NVIC_InitTypeDef NVIC_InitStructure;//�ṹ������һ�μ���
    NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn;//�ж�0������
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�Ϊ1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;//��Ӧ���ȼ�Ϊ1
    
    NVIC_InitStructure.NVIC_IRQChannel=EXTI1_IRQn;//�ж�1������
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=2 ;//��Ӧ���ȼ�Ϊ2�����ж�1�����ȼ��Ե����ж�0�����ȼ�
    NVIC_Init(&NVIC_InitStructure);//NVIC��ʼ��

    
}

int16_t Return_Encoder_Count(void) //����Encoder_Count��ֵ�������ת�ͷ���+1����ת�ͷ���-1�����㹲����������
{
    int16_t Temp;
    Temp=Encoder_Count;              
    Encoder_Count=0;
    return Temp;
    
}

//�ж�0����
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetFlagStatus(EXTI_Line0)==SET)
    {
        EXTI_ClearITPendingBit(EXTI_Line0);//����жϱ�־λ���Ա���һ�ν����ж�
        if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==1)  //�����ת
        {
            Encoder_Count++;  
        }
        
    }
    
}
//�ж�1����
void EXTI1_IRQHandler(void)
{
    if(EXTI_GetFlagStatus(EXTI_Line1)==SET)
    {
        EXTI_ClearITPendingBit(EXTI_Line1);
        if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==1)  //�����ת
        {
            Encoder_Count--;
        }
    }
    
}

