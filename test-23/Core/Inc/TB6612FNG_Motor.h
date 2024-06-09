/**
 *  @file TB6612FNG_Motor.h
 *  @brief Header file for TB6612FNG_Motor class.
 *  @details This file contains the implementation of the TB6612FNG_Motor class, which provides functions for controlling a DC motor using the TB6612FNG motor driver. It includes functions for setting the motor duty cycle, enabling and disabling the motor, and setting the inverted compare value.
 *  @author Max Gardenswartz 
 *  @date May 18, 2024
 **/

#ifndef SRC_TB6612FNGMotor_H_
#define SRC_TB6612FNGMotor_H_

#include "stm32f4xx_hal.h" // #include <cstdint>

class TB6612FNG_Motor
{
	public:
		// Class attributes
		GPIO_TypeDef* IN1_pin_group;
		uint16_t IN1_pin;
		GPIO_TypeDef* IN2_pin_group;
		uint16_t IN2_pin;
		TIM_HandleTypeDef* timer_handle;
		uint32_t timer_channel;
		uint32_t CPU_clock_speed_MHz;
		uint16_t PWM_frequency_Hz;

		uint16_t auto_reload_value;

		// Methods
		TB6612FNG_Motor(
				GPIO_TypeDef* IN1_pin_group, uint16_t IN1_pin,
				GPIO_TypeDef* IN2_pin_group, uint16_t IN2_pin,
				TIM_HandleTypeDef* timer_handle, uint32_t timer_channel,
				uint32_t CPU_clock_speed_MHz, uint32_t PWM_frequency_Hz = 20000);
		virtual ~TB6612FNG_Motor();
		void set_duty(int16_t duty, int16_t maximum_value = 100);
		void start();
		void stop();

	private:

};

#endif /* SRC_TB6612FNGMotor_H_ */