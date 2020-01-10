/**********************************************
File Name: ALIENTEK MINISTM32 实验1 跑马灯(库)
Revision:
Corporation:
Microcontroller Corporation:ST
Microcontroller name:STM32F103RBT6 ARM
Compiler: Keil MDK_ARM 4.11
Author: moweidong
E-mail: moweidongsabbit@21cn.com
Date : 2010.12.05
*********************************************/
//仿真仪器设备:
//ALIENTEK_MiniSTM32_REVB0开发板
//JLINK V7
/*********************************************
//库
//CMSIS:V1.20
//STM32F10x_StdPeriph_Driver:V3.1.1
*********************************************/

//本程序使用了printf用于在LCD和串口显示数据,因些对Option for Target属性有如下更改
//Target选项页:勾中Use MicroLIB(使用微库)

//LED0:PA8
//LED1:PD2

//注意:在禁用JTAG接后后可这样恢复:
//1.设置KEIL为JTAG或SW下载模式
//2.给板上电,按下板上的复位按键不放,点击KEIL下的FLASH下载按钮,使JTAG或SW下载工具检测完成接口后立刻放开,
//此目的在于争取IC在上电的时候有控制权,使在未进入禁用JTAG或SW生效的时候取可控制IC进行烧录

#include "stm32f10x.h"
#include "LCD.h"
#include "eval.h"
#include "SysTickDelay.h"
#include "UART_INTERFACE.h"
#include <stdio.h>




/* Private typedef -----------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);


//GPIO引脚配置
void GPIO_Configuration(void)
	{
	GPIO_InitTypeDef GPIO_InitStructure;
				
//	Configure(PA.10) as alternate function floating 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
/*
	Configure USART1 Rx (PA.10) as input push-pull 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
*/
	/* Initialize LEDs and Key Button mounted on STM3210X-EVAL board */       
	STM_EVAL_LEDInit1(LED1);  //pp
	STM_EVAL_LEDInit2(LED2);  //pp 
	}

//系统中断管理
void NVIC_Configuration(void)
	{ 
	//NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	//设置优先级分组：先占优先级0位,从优先级4位
	
	//设置向量表的位置和偏移
	#ifdef  VECT_TAB_RAM  
		/* Set the Vector Table base location at 0x20000000 */ 
		NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 		//向量表位于RAM
	#else  /* VECT_TAB_FLASH  */
		/* Set the Vector Table base location at 0x08000000 */ 
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   //向量表位于FLASH
	#endif
	}

//配置系统时钟,使能各外设时钟
void RCC_Configuration(void)
	{
	SystemInit();	
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA 
	//	|RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC
	//	|RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE
	//	|RCC_APB2Periph_ADC1  | RCC_APB2Periph_AFIO 
	//	|RCC_APB2Periph_SPI1, ENABLE );
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_ALL ,ENABLE );
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 
	//	|RCC_APB1Periph_USART3|RCC_APB1Periph_TIM2	                           
	//	, ENABLE );
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	}
	





//配置所有外设
void Init_All_Periph(void)
	{
	
	RCC_Configuration();	
	
//	NVIC_Configuration();

	GPIO_Configuration();
	
//	USART_Configuration(9600);
	}



void Delay(vu32 nCount)
	{
	for(; nCount != 0; nCount--);
	}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
//void Delay(__IO uint32_t nCount)
//	{
//	for(; nCount != 0; nCount--);
//	}
				
void DelayS(unsigned int s)
{
    unsigned char i;
    for(i=0;i<s;i++)
    {
        delay_ms(1000);
    }
}

uint8_t wait_for_calculate(void)
	{
		int chance[6]=0;
		
		DelayS(15); 
		chance[0]= GPIO_ReadInputDataBit(GPIOC, LED1_GPIO_PIN);    // 1
		STM_EVAL_LEDOn(LED1);
		delay_ms(60);
		STM_EVAL_LEDOff(LED1);
		delay_ms(60);
		STM_EVAL_LEDOn(LED1);
		delay_ms(60);
		STM_EVAL_LEDOff(LED1);
		delay_ms(60);
		
		DelayS(15);
		chance[1]= GPIO_ReadInputDataBit(GPIOC, LED1_GPIO_PIN);  // 2
		STM_EVAL_LEDOn(LED1);
		delay_ms(60);
		STM_EVAL_LEDOff(LED1);
		delay_ms(60);
		STM_EVAL_LEDOn(LED1);
		delay_ms(60);
		STM_EVAL_LEDOff(LED1);
		delay_ms(60);
		
		DelayS(15);
		chance[2]= GPIO_ReadInputDataBit(GPIOC, LED1_GPIO_PIN);  // 3
		STM_EVAL_LEDOn(LED1);
		delay_ms(60);
		STM_EVAL_LEDOff(LED1);
		delay_ms(60);
		STM_EVAL_LEDOn(LED1);
		delay_ms(60);
		STM_EVAL_LEDOff(LED1);
		delay_ms(60);			
		
		DelayS(15);
		chance[3]= GPIO_ReadInputDataBit(GPIOC, LED1_GPIO_PIN);  // 4
		STM_EVAL_LEDOn(LED1);
		delay_ms(60);
		STM_EVAL_LEDOff(LED1);
		delay_ms(60);
		STM_EVAL_LEDOn(LED1);
		delay_ms(60);
		STM_EVAL_LEDOff(LED1);
		delay_ms(60);
		
		DelayS(15);
		chance[4]= GPIO_ReadInputDataBit(GPIOC, LED1_GPIO_PIN);  // 5
		STM_EVAL_LEDOn(LED1);
		delay_ms(60);
		STM_EVAL_LEDOff(LED1);
		delay_ms(60);
		STM_EVAL_LEDOn(LED1);
		delay_ms(60);
		STM_EVAL_LEDOff(LED1);
		delay_ms(60);
		
		DelayS(15);
		chance[5]= GPIO_ReadInputDataBit(GPIOC, LED1_GPIO_PIN);  // 6
		STM_EVAL_LEDOn(LED1);
		delay_ms(60);
		STM_EVAL_LEDOff(LED1);
		delay_ms(60);
		STM_EVAL_LEDOn(LED1);
		delay_ms(60);
		STM_EVAL_LEDOff(LED1);
		delay_ms(60);
		
		return chance[0]+chance[1]+chance[2]+chance[3]+chance[4]+chance[5];
			
}

int main(void)
{  
	uint8_t num;
	Init_All_Periph();
	SysTick_Initaize();
		
	STM_EVAL_LEDOn(LED1);  // 状态指示灯常亮
	STM_EVAL_LEDOff(LED2);		//输出高电平，回位
		 
	while(1)
		{
			if(GPIO_ReadInputDataBit(GPIOC,LED1_GPIO_PIN) == 0x00) //出问题
				{
					num = wait_for_calculate();  //状态灯闪烁
					if(num <= 1)
							//无应答，说明断电了，开机
						{
							num=0;
							STM_EVAL_LEDOn(LED2);		//输出低电平，接通继电器,他们同时点亮
							delay_ms(700); 
							STM_EVAL_LEDOff(LED1);  // 状态指示灯常灭
							STM_EVAL_LEDOff(LED2);		//输出高电平，回位
						}
					else //误判了
						{
							STM_EVAL_LEDOff(LED2);		//输出高电平，回位
						}
					
				}
				
			else  //没有出现问题
				{
					STM_EVAL_LEDOn(LED1);  // 状态指示灯常亮
				}
				
				DelayS(60); //每隔60s检测一次
			
			
		}
	
}

//	GPIO_PORT[Led]->BRR = GPIO_PIN[Led];		//清除


/*




int main(void)
	{  
	int chance[6]=0;
	Init_All_Periph();
	SysTick_Initaize();
	GPIOA->BRR = GPIO_Pin_13;		//输出高电平，回位
	STM_EVAL_LEDOff(LED1);
	STM_EVAL_LEDOff(LED2);
	while(1)
		{
			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10) == 0x00);  // read GPIO
			{	
				DelayS(1);
				chance[0]= GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10);    // 1
				STM_EVAL_LEDOn(LED1);
				STM_EVAL_LEDOn(LED2);
				delay_ms(60);
				STM_EVAL_LEDOff(LED1);
				STM_EVAL_LEDOff(LED2);
				delay_ms(60);
				STM_EVAL_LEDOn(LED1);
				STM_EVAL_LEDOn(LED2);
				delay_ms(60);
				STM_EVAL_LEDOff(LED1);
				STM_EVAL_LEDOff(LED2);
				delay_ms(60);
				
				DelayS(1);
				chance[1]= GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10);  // 2
				STM_EVAL_LEDOn(LED1);
				STM_EVAL_LEDOn(LED2);
				delay_ms(60);
				STM_EVAL_LEDOff(LED1);
				STM_EVAL_LEDOff(LED2);
				delay_ms(60);
				STM_EVAL_LEDOn(LED1);
				STM_EVAL_LEDOn(LED2);
				delay_ms(60);
				STM_EVAL_LEDOff(LED1);
				STM_EVAL_LEDOff(LED2);
				delay_ms(60);
				
				DelayS(1);
				chance[2]= GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10);  // 3
				STM_EVAL_LEDOn(LED1);
				STM_EVAL_LEDOn(LED2);
				delay_ms(60);
				STM_EVAL_LEDOff(LED1);
				STM_EVAL_LEDOff(LED2);
				delay_ms(60);
				STM_EVAL_LEDOn(LED1);
				STM_EVAL_LEDOn(LED2);
				delay_ms(60);
				STM_EVAL_LEDOff(LED1);
				STM_EVAL_LEDOff(LED2);
				delay_ms(60);			
				
				DelayS(1);
				chance[3]= GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10);  // 4
				STM_EVAL_LEDOn(LED1);
				STM_EVAL_LEDOn(LED2);
				delay_ms(60);
				STM_EVAL_LEDOff(LED1);
				STM_EVAL_LEDOff(LED2);
				delay_ms(60);
				STM_EVAL_LEDOn(LED1);
				STM_EVAL_LEDOn(LED2);
				delay_ms(60);
				STM_EVAL_LEDOff(LED1);
				STM_EVAL_LEDOff(LED2);
				delay_ms(60);
				
				DelayS(1);
				chance[4]= GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10);  // 5
				STM_EVAL_LEDOn(LED1);
				STM_EVAL_LEDOn(LED2);
				delay_ms(60);
				STM_EVAL_LEDOff(LED1);
				STM_EVAL_LEDOff(LED2);
				delay_ms(60);
				STM_EVAL_LEDOn(LED1);
				STM_EVAL_LEDOn(LED2);
				delay_ms(60);
				STM_EVAL_LEDOff(LED1);
				STM_EVAL_LEDOff(LED2);
				delay_ms(60);
				
				DelayS(1);
				chance[5]= GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10);  // 6
				STM_EVAL_LEDOn(LED1);
				STM_EVAL_LEDOn(LED2);
				delay_ms(60);
				STM_EVAL_LEDOff(LED1);
				STM_EVAL_LEDOff(LED2);
				delay_ms(60);
				STM_EVAL_LEDOn(LED1);
				STM_EVAL_LEDOn(LED2);
				delay_ms(60);
				STM_EVAL_LEDOff(LED1);
				STM_EVAL_LEDOff(LED2);
				delay_ms(60);
				
				if(chance[0]+chance[1]+chance[2]+chance[3]+chance[4]+chance[5] >= 4)
					//说明断电了，开机
				{
					GPIOA->BSRR = GPIO_Pin_13;		//输出低电平，接通继电器
					DelayS(3); 
					GPIOA->BRR = GPIO_Pin_13;		//输出高电平，回位
					
					chance[0]=0;
					chance[1]=0;
					chance[2]=0;
					chance[3]=0;
					chance[4]=0;
					chance[5]=0;
					
				}
			}
			
		//一切正常
		STM_EVAL_LEDOn(LED1);
		STM_EVAL_LEDOn(LED2);
//		DelayS(14400);  //4h
//		DelayS(20);  
			
		}
	
}*/


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif





