//=============================================================================
//�ļ����ƣ�main.h
//���ܸ�Ҫ��STM32F103C8���ļ��
//��Ȩ���У�Դ�ع�����www.vcc-gnd.com
//��Ȩ���£�2013-02-20
//���Է�ʽ��J-Link OB ARM SW��ʽ 5MHz
//=============================================================================

//ͷ�ļ�
#include "stm32f10x.h"
#include "GPIOLIKE51.h"

//��������
void GPIO_Configuration(void);

//=============================================================================
//�ļ����ƣ�Delay
//���ܸ�Ҫ����ʱ
//����˵����nCount����ʱ����
//�������أ���
//=============================================================================

void Delay(uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

int16_t Number;


int16_t Encoder_Count;//����һ�������ŵı���

void Encoder_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;//�ṹ������һ�μ���
    //����RCC
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    
    //����GPIO
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;//ʹ�õ�PB0��PB1��
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    
    //����AFIO
    GPIO_EXTILineConfig(RCC_APB2Periph_GPIOB,GPIO_PinSource0);
    GPIO_EXTILineConfig(RCC_APB2Periph_GPIOB,GPIO_PinSource1); //AFIOÿ���ж��Ƿֿ���
    
    //����EXTI
    EXTI_InitStructure.EXTI_Line=EXTI_Line1|EXTI_Line0;//ʹ�õ��ж�0���ж�1
    EXTI_InitStructure.EXTI_LineCmd=ENABLE;
    EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling; //�½��ش���
    EXTI_Init(&EXTI_InitStructure);
    
    //����NVIC�����������������ж�
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����һ�μ��ɣ�ֻ����һ�����ж�����뱣֤��ͬ
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


//=============================================================================
//�ļ����ƣ�main
//���ܸ�Ҫ��������
//����˵������
//�������أ�int
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
//�ļ����ƣ�GPIO_Configuration
//���ܸ�Ҫ��GPIO��ʼ��
//����˵������
//�������أ���
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



