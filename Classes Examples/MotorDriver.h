/*
 * MotorDriver.h
 *
 *  Created on: Apr 26, 2024
 *      Author: Max Gardenswartz
 */

#ifndef SRC_MOTORDRIVER_H_
#define SRC_MOTORDRIVER_H_

#include "stm32l4xx_hal.h"
#include <cstdint>
// #include <cassert>

class MotorDriver {
public:
	// Class attributes
	TIM_HandleTypeDef* timer;
	GPIO_TypeDef* pin_1_group;
	uint16_t pin_1;
	uint32_t pin_1_timer_channel;
	GPIO_TypeDef* pin_2_group;
	uint16_t pin_2;
	uint16_t pin_2_timer_channel;

	// Methods
	MotorDriver(TIM_HandleTypeDef* timer,
			GPIO_TypeDef* pin_1_group, uint16_t pin_1, uint32_t pin_1_timer_channel,
			GPIO_TypeDef* pin_2_group, uint16_t pin_2, uint32_t pin_2_timer_channel);
	virtual ~MotorDriver();
	void set_duty(float duty);
	void initialize();

private:
};

#endif /* SRC_MOTORDRIVER_H_ */
