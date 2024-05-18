/*
 * MotorDriver.cpp
 *
 *  Created on: Apr 26, 2024
 *      Author: Max Gardenswartz
 */

#include "MotorDriver.h"

MotorDriver::MotorDriver(TIM_HandleTypeDef* timer,
	GPIO_TypeDef* pin_1_group, uint16_t pin_1, uint32_t pin_1_timer_channel,
	GPIO_TypeDef* pin_2_group, uint16_t pin_2, uint32_t pin_2_timer_channel)
		: timer(timer),
		  pin_1_group(pin_1_group), pin_1(pin_1), pin_1_timer_channel(pin_1_timer_channel),
		  pin_2_group(pin_2_group), pin_2(pin_2), pin_2_timer_channel(pin_2_timer_channel) {
	initialize();
}

MotorDriver::~MotorDriver() {
	// TODO Auto-generated destructor stub
}

void MotorDriver::set_duty(float duty){
	// Calculate compare value.
	uint32_t auto_reload_value = timer->Init.Period;
	int compare_value = (100-duty)*auto_reload_value/100;

	// Update duty cycle
	if(duty>=0){
		// Raise pin 1
		__HAL_TIM_SET_COMPARE(timer, pin_1_timer_channel, auto_reload_value);

		// Update duty cycle on pin 2
		__HAL_TIM_SET_COMPARE(timer, pin_2_timer_channel, compare_value);

	} else {
		// Raise pin 2
		__HAL_TIM_SET_COMPARE(timer, pin_2_timer_channel, auto_reload_value);

		// Update duty cycle on pin 1
		__HAL_TIM_SET_COMPARE(timer, pin_1_timer_channel, compare_value);
	}
}
void MotorDriver::initialize(){
	// Motor stop
	HAL_TIM_PWM_Start(timer,pin_1_timer_channel);
	HAL_TIM_PWM_Start(timer,pin_2_timer_channel);
	__HAL_TIM_SET_COMPARE(timer, pin_1_timer_channel, 0);
	__HAL_TIM_SET_COMPARE(timer, pin_2_timer_channel, 0);
}


