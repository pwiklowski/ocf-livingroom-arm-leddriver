/**************************************************************************
 *
 *      Autor: Pawel Wiklowski
 *
 *      pawwik@gmail.com
 *
 *************************************************************************/
#include "gpio.h"


void gpio_config(GPIO_TypeDef* gport, GPIOSpeed_TypeDef speed, GPIOMode_TypeDef mode, uint16_t gpios){
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = gpios;
	GPIO_InitStructure.GPIO_Mode = mode;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = speed;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(gport, &GPIO_InitStructure);
}

void gpio_set(GPIO_TypeDef* gport, uint16_t gpios)
{
	gport->BSRR = gpios;
}

void gpio_clear(GPIO_TypeDef* gport, uint16_t gpios)
{
	gport->BRR = gpios;
}

uint16_t gpio_get(GPIO_TypeDef* gport, uint16_t gpios)
{
	return gport->IDR & gpios;
}

void gpio_toggle(GPIO_TypeDef* gport, uint16_t gpio)
{
	if (((gport->IDR) & gpio) == gpio) gpio_clear(gport, gpio);
	else gpio_set(gport, gpio);
}
