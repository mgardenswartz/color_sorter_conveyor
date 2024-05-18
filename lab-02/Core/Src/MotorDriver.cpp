/*
 * MotorDriver.cpp
 *
 *  Created on: Apr 26, 2024
 *  Updated Mat 15, 2024
 *      Author: Max Gardenswartz
 */

#include "MotorDriver.h"

MotorDriver::MotorDriver(TIM_HandleTypeDef* timer,
	GPIO_TypeDef* pin_1_group, uint16_t pin_1, uint32_t pin_1_timer_channel,
	GPIO_TypeDef* pin_2_group, uint16_t pin_2, uint32_t pin_2_timer_channel)
		: timer(timer),
		  pin_1_group(pin_1_group), pin_1(pin_1), pin_1_timer_channel(pin_1_timer_channel),
		  pin_2_group(pin_2_group), pin_2(pin_2), pin_2_timer_channel(pin_2_timer_channel)
{
	// Start the PWM generation
	enable();

	// Motor stop
	__HAL_TIM_SET_COMPARE(timer, pin_1_timer_channel, 0);
	__HAL_TIM_SET_COMPARE(timer, pin_2_timer_channel, 0);
}

MotorDriver::~MotorDriver()
{
	// TODO Auto-generated destructor stub
}

void MotorDriver::set_duty(float duty)
{
	// Set the auto_reload_value
	uint32_t auto_reload_value = timer->Init.Period;

	// Update duty cycle
	if(duty>=0){
		int compare_value = (100-duty)*auto_reload_value/100;

		// Raise pin 1
		__HAL_TIM_SET_COMPARE(timer, pin_1_timer_channel, auto_reload_value);

		// Update duty cycle on pin 2
		__HAL_TIM_SET_COMPARE(timer, pin_2_timer_channel, compare_value);

	}
	else
	{
		// Absolute value
		duty *= -1;
		int compare_value = (100-duty)*auto_reload_value/100;

		// Raise pin 2
		__HAL_TIM_SET_COMPARE(timer, pin_2_timer_channel, auto_reload_value);

		// Update duty cycle on pin 1
		__HAL_TIM_SET_COMPARE(timer, pin_1_timer_channel, compare_value);
	}
}

void MotorDriver::enable(){

	// Start PWM Generation
	HAL_TIM_PWM_Start(timer, pin_1_timer_channel);
	HAL_TIM_PWM_Start(timer, pin_2_timer_channel);
}
void MotorDriver::disable()
{
	// Stop PWM Generation
	HAL_TIM_PWM_Stop(timer, pin_1_timer_channel);
	HAL_TIM_PWM_Stop(timer, pin_2_timer_channel);

	// Motor stop
	__HAL_TIM_SET_COMPARE(timer, pin_1_timer_channel, 0);
	__HAL_TIM_SET_COMPARE(timer, pin_2_timer_channel, 0);
}
