#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stm32f0xx.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_usart.h"
#include "gpio.h"



void setOutput(uint8_t output, uint16_t value){


	switch(output){
	case 1:
		TIM3->CCR1 = value;
		break;
	case 2:
		TIM3->CCR2 = value;
		break;
	case 3:
		TIM3->CCR4 = value;
		break;
	case 4:
		TIM1->CCR2 = value;
		break;
	case 5:
		TIM1->CCR3 = value;
		break;
	case 6:
		TIM14->CCR1 = value;
		break;

	}

}
void setup(){
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOAEN, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOBEN, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = 4;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period =  10000;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;

	TIM_OCInitTypeDef outputChannelInit;
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	outputChannelInit.TIM_Pulse = 8000;
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OutputNState = TIM_OutputNState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	outputChannelInit.TIM_OCNPolarity = TIM_OCNPolarity_High;
	outputChannelInit.TIM_OCIdleState = TIM_OCIdleState_Set;
	outputChannelInit.TIM_OCNIdleState = TIM_OCNIdleState_Set;

	TIM_TimeBaseInit(TIM1, &timerInitStructure);
	TIM_TimeBaseInit(TIM3, &timerInitStructure);
	TIM_TimeBaseInit(TIM14, &timerInitStructure);

	TIM_OC1Init(TIM3, &outputChannelInit);
	TIM_OC2Init(TIM3, &outputChannelInit);
	TIM_OC4Init(TIM3, &outputChannelInit);

	TIM_OC3Init(TIM1, &outputChannelInit);
	TIM_OC2Init(TIM1, &outputChannelInit);

	TIM_OC1Init(TIM14, &outputChannelInit);

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);


	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);


	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);

	GPIO_InitTypeDef gpioStructure;
	gpioStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_10;
	gpioStructure.GPIO_Mode = GPIO_Mode_AF;
	gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
	gpioStructure.GPIO_PuPd = GPIO_PuPd_UP;
	gpioStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOA, &gpioStructure);


	gpioStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB, &gpioStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_1);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_2);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_4);

	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM1, ENABLE);
	TIM_Cmd(TIM14, ENABLE);

	TIM_CtrlPWMOutputs(TIM1, ENABLE);


	//USART
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_USART1EN, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);

	USART_Cmd(USART1, ENABLE);
}

uint8_t readChar(){
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	return (USART_ReceiveData(USART1) & 0x7F);

}
uint8_t isNumber(char* str){
	char* p = str;
	while (*p != 0 ){
		if (*p > '9') return 0;
		if (*p < '0') return 0;
		p++;
	}
	return 1;
}
int my_atoi(const char *c)
{
    long long int value = 0;
    int sign = 1;
    if( *c == '+' || *c == '-' )
    {
        if( *c == '-' ) sign = -1;
        c++;
    }
    while (isdigit(*c))
    {
        value *= 10;
        value += (int) (*c-'0');
        c++;
    }
    return (value * sign);
}
void parseCommand(char* cmd, char* value){
	if (isNumber(value) == 0) return;

	uint16_t v = my_atoi(value);

	if (strcmp ("1", cmd) == 0){
		setOutput(1, v);
	} else if (strcmp ("2", cmd) == 0){
		setOutput(2, v);
	} else if (strcmp ("3", cmd) == 0){
		setOutput(3, v);
	} else if (strcmp ("4", cmd) == 0){
		setOutput(4, v);
	} else if (strcmp ("5", cmd) == 0){
		setOutput(5, v);
	} else if (strcmp ("6", cmd) == 0){
		setOutput(6, v);
	}
}

void sendChar(char c){
	USART_SendData(USART1, c);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}



int main()
{
	setup();

	setOutput(1, 0);
	setOutput(2, 0);
	setOutput(3, 0);
	setOutput(4, 0);
	setOutput(5, 0);
	setOutput(6, 0);

	char cmd[30];
	char value[30];

	int part = 0;
	int i=0;

	while (1)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
		char c = (USART_ReceiveData(USART1) & 0x7F);

		sendChar(c);

		if (c == '\n'){
			value[i] = 0;
			i=0;
			if (part==1) parseCommand(cmd, value);

			part=0;
		}
		else if (c == ' '){
			part++;
			cmd[i] = 0;
			i = 0;
		}
		else if (c == '\r'){
			//ignore
		}
		else{
			if (i<20){
				if (part==0)cmd[i++] = c;
				if (part==1)value[i++] = c;
			}
		}



	}
	return 0;
}
