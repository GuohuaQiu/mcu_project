/**************************************************************************/ /**
                                                                              * @file     main.c
                                                                              * @version  V3.00
                                                                              * @brief    Show how to set GPIO pin mode and use pin data input/output control.
                                                                              *
                                                                              * @copyright (C) 2013~2015 Nuvoton Technology Corp. All rights reserved.
                                                                              *
                                                                              ******************************************************************************/
#include "stdio.h"
#include "NuMicro.h"

#define PLL_CLOCK 192000000

/**
 * @brief       GPIO PC IRQ
 *
 * @param       None
 *
 * @return      None
 *
 * @details     The PC default IRQ, declared in startup_M480.s.
 */
void GPC_IRQHandler(void)
{
    /* To check if PC.5 interrupt occurred */
    if(GPIO_GET_INT_FLAG(PC, BIT5))
    {
        GPIO_CLR_INT_FLAG(PC, BIT5);
        printf("PC.5 INT occurred.\n");
    }
    else
    {
        /* Un-expected interrupt. Just clear all PC interrupts */
        PC->INTSRC = PC->INTSRC;
        printf("Un-expected interrupts.\n");
    }
}

void SYS_Init(void)
{

    /* Set XT1_OUT(PF.2) and XT1_IN(PF.3) to input mode */
    PF->MODE &= ~(GPIO_MODE_MODE2_Msk | GPIO_MODE_MODE3_Msk);

    /* Enable HXT clock (external XTAL 12MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);

    /* Wait for HXT clock ready */
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(PLL_CLOCK);

    /* Set PCLK0/PCLK1 to HCLK/2 */
    CLK->PCLKDIV = (CLK_PCLKDIV_APB0DIV_DIV2 | CLK_PCLKDIV_APB1DIV_DIV2);

    /* Enable UART module clock */
    CLK_EnableModuleClock(UART0_MODULE);

    /* Select UART module clock source as HXT and UART module clock divider as 1 */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HXT, CLK_CLKDIV0_UART0(1));

    // /* Set GPB multi-function pins for UART0 RXD and TXD */
    // SYS->GPB_MFPH &= ~(SYS_GPB_MFPH_PB12MFP_Msk | SYS_GPB_MFPH_PB13MFP_Msk);
    // SYS->GPB_MFPH |= (SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);
    /* Set GPB multi-function pins for UART0 RXD and TXD */
    SYS->GPA_MFPL &= ~(SYS_GPA_MFPL_PA1MFP_Msk);
    SYS->GPA_MFPL |= (SYS_GPA_MFPL_PA1MFP_UART0_TXD);
}

void UART0_Init()
{
    /* Configure UART0 and set UART0 baud rate */
    UART_Open(UART0, 115200);
}


int32_t main(void)
{

    int32_t i32Err, i32TimeOutCnt;

    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, peripheral clock and multi-function I/O */
    SYS_Init();

    /* Lock protected registers */
    SYS_LockReg();

    /* Init UART0 for printf */
    UART0_Init();

    printf("\n\nCPU @ %dHz\n", SystemCoreClock);

    printf("+-------------------------------------------------+\n");
    printf("|    PB.3(Output) and PD.8(Input) Sample Code     |\n");
    printf("+-------------------------------------------------+\n\n");

    /*-----------------------------------------------------------------------------------------------------*/
    /* GPIO Basic Mode Test --- Use Pin Data Input/Output to control GPIO pin                              */
    /*-----------------------------------------------------------------------------------------------------*/
    printf("  >> Please connect PB.3 and PD.8 first << \n");
    printf("     Press any key to start test by using [Pin Data Input/Output Control] \n\n");

    /* Configure PB.3 as Output mode and PD.8 as Input mode then close it */

    // GPIO_SetMode(PD, BIT8, GPIO_MODE_INPUT);

    GPIO_SetMode(PE, BIT5, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PE, BIT4, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PC, BIT11, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PC, BIT12, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PC, BIT10, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PC, BIT9, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PB, BIT3, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PB, BIT2, GPIO_MODE_OUTPUT);

    // GPIO_SetMode(PB, BIT6, GPIO_MODE_OUTPUT);
    // GPIO_SetMode(PB, BIT7, GPIO_MODE_OUTPUT);
    // GPIO_SetMode(PB, BIT8, GPIO_MODE_OUTPUT);
    // GPIO_SetMode(PB, BIT9, GPIO_MODE_OUTPUT);
    // GPIO_SetMode(PB, BIT0, GPIO_MODE_OUTPUT);
    // GPIO_SetMode(PB, BIT1, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PB, BIT3, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PB, BIT4, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PB, BIT5, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PB, BIT0, GPIO_MODE_OUTPUT);


    printf("GPIO PB.3(output mode) connect to PD.8(input mode) ......");

    /* Configure PC.5 as Quasi-bidirection mode and enable interrupt by falling edge trigger */
    GPIO_SetMode(PC, BIT5, GPIO_MODE_INPUT);
    GPIO_EnableInt(PC, 5, GPIO_INT_FALLING);
    NVIC_EnableIRQ(GPC_IRQn);

    /* Enable interrupt de-bounce function and select de-bounce sampling cycle time is 1024 clocks of LIRC clock */
    GPIO_SET_DEBOUNCE_TIME(GPIO_DBCTL_DBCLKSRC_LIRC, GPIO_DBCTL_DBCLKSEL_1024);
    GPIO_ENABLE_DEBOUNCE(PC, BIT5);
    // OUT_DIGIT(7);
    while (1)
    {
        a_show();
        a_work();
        // ShowDigit(1);
        // b_finding();
    }
}