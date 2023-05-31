/**************************************************************************
 * @file     main.c
 * @version  V3.00
 * @brief    Show how to set GPIO pin mode and use pin data input/output control.
 *
 * @copyright (C) 2013~2015 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include "stdio.h"
#define PLATFORM_STM
#ifdef PLATFORM_STM
#include "stm32f10x.h"
#include "GPIOLIKE51.h"
#else
#include "NuMicro.h"
#endif

#define PLL_CLOCK 192000000
// #define SHARE_VCC
#ifdef SHARE_VCC
// int share_mode[] = {0xC0, 0xF9, 0xA4, 0xb0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E};
#define TURN_ON   1
#define TURN_OFF  0
#else
#define TURN_ON   0
#define TURN_OFF  1
#endif
int share_mode[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
#ifdef PLATFORM_STM
#define LED_PART_A PAout(0)
#define LED_PART_B PAout(1)
#define LED_PART_C PAout(2)
#define LED_PART_D PAout(3)
#define LED_PART_E PAout(4)
#define LED_PART_F PAout(5)
#define LED_PART_G PAout(6)

#define LED_CHANNEL_A PBout(5)
#define LED_CHANNEL_B PBout(6)
#define LED_CHANNEL_C PBout(7)
#define LED_CHANNEL_D PBout(8)

#else
#define LED_PART_A PE4
#define LED_PART_B PC12
#define LED_PART_C PC11
#define LED_PART_D PC10
#define LED_PART_E PC9
#define LED_PART_F PB3
#define LED_PART_G PB2

#define LED_CHANNEL_A PB0
#define LED_CHANNEL_B PB1
#define LED_CHANNEL_D PB8
#define LED_CHANNEL_C PB9

#endif

#define  SWITCH_INTERVAL 10;

int g_index = 0;
volatile uint8_t g_a[4] = { 6,7,8,9};

volatile uint32_t g_timer = SWITCH_INTERVAL;
#define CHANGE_INTERVAL 1000000;
uint32_t g_work_timer = CHANGE_INTERVAL;

static void OUT_STAMP(uint8_t y)
{
    LED_PART_A = y & 0x1 ? TURN_OFF : TURN_ON;
    LED_PART_B = y & 0x2 ? TURN_OFF : TURN_ON;
    LED_PART_C = y & 0x4 ? TURN_OFF : TURN_ON;
    LED_PART_D = y & 0x8 ? TURN_OFF : TURN_ON;
    LED_PART_E = y & 0x10 ? TURN_OFF : TURN_ON;
    LED_PART_F = y & 0x20 ? TURN_OFF : TURN_ON;
    LED_PART_G = y & 0x40 ? TURN_OFF : TURN_ON;
}

void OUT_DIGIT(uint8_t x)
{
    if (x > 0xf)
    {
        OUT_STAMP(0);
    }
    else
    {
        OUT_STAMP(share_mode[x]);
    }
}

int pos = 0;


void SetPos(uint8_t d)
{
    switch (d)
    {
    case 0:
        LED_CHANNEL_A = TURN_ON;
        LED_CHANNEL_B = TURN_OFF;
        LED_CHANNEL_C = TURN_OFF;
        LED_CHANNEL_D = TURN_OFF;
        break;
    case 1:
        LED_CHANNEL_A = TURN_OFF;
        LED_CHANNEL_B = TURN_ON;
        LED_CHANNEL_C = TURN_OFF;
        LED_CHANNEL_D = TURN_OFF;
        break;
    case 2:
        LED_CHANNEL_A = TURN_OFF;
        LED_CHANNEL_B = TURN_OFF;
        LED_CHANNEL_C = TURN_ON;
        LED_CHANNEL_D = TURN_OFF;
        break;
    case 3:
        LED_CHANNEL_A = TURN_OFF;
        LED_CHANNEL_B = TURN_OFF;
        LED_CHANNEL_C = TURN_OFF;
        LED_CHANNEL_D = TURN_ON;
        break;
    default:
        break;
    }
}


void Switch()
{
    uint8_t d = pos++ % 4;
    SetPos(d);
}
void ShowDigit(uint32_t x)
{
    uint8_t d = x % 16;
    OUT_STAMP(share_mode[d]);
    Switch();
}
int global = 0;
void ShowMe()
{
    int x = 100000000;
    while (x--)
        ;
    ShowDigit(global);
    global++;
    printf("change:%d", global);
}

#define FREQ 2
int freq = 0;
void SwitchPos()
{
    freq++;
    if (freq > 0)
    {
        if (freq > FREQ)
        {
            freq = -1;
        }
        OUT_DIGIT(0xff);
        return;
    }

    SetPos(g_index);

    OUT_DIGIT(g_a[g_index]);
    g_index++;
    if (g_index >= 4)
    {
        g_index = 0;
    }

#if 0
    switch (g_index)
    {
    case 0:
        printf("%x - - -\n", g_a[g_index]);
        break;
    case 1:
        printf("- %x - -\n", g_a[g_index]);
        break;
    case 2:
        printf("- - %x -\n", g_a[g_index]);
        break;
    case 3:
        printf("- - - %x\n", g_a[g_index]);
        break;
    
    default:
        break;
    }
#endif
}


void a_work()
{
    int i = 0;
    int result = 0;
    g_work_timer--;
    if (g_work_timer > 0)
    {
        return;
    }
    g_work_timer = CHANGE_INTERVAL;

    g_a[1]++;

    if (g_a[1] > 9)
    {
        g_a[0]++;
        if (g_a[0] > 9)
        {
            g_a[0] = 1;
            g_a[1] = 1;
        }
        else
        {
            g_a[1] = g_a[0];
            if (g_a[0] > 9)
            {
                g_a[0] = g_a[1];
            }
        }
    }

    result = g_a[0] * g_a[1];
    g_a[2] = result / 10;
    g_a[3] = result % 10;
}

void a_show()
{
    g_timer--;
    if (g_timer > 0)
    {
        return;
    }
    g_timer = SWITCH_INTERVAL;
    SwitchPos();
}

#define TEST_INTERVAL 4000000
void b_finding()
{
    static int x = 0;
    g_timer--;
    if (g_timer > 0)
    {
        return;
    }
    g_timer = TEST_INTERVAL;
    // if (x == 0)
    // {
    //     PB3 = TURN_OFF;
    //     PB2 = TURN_ON;
    //     x = 1;
    //     printf("ff .\n");
    // }
    // else
    {
        // PB2 = TURN_OFF;
        // PB3 = TURN_ON;
        x = 0;
        printf("on .\n");
    }
}