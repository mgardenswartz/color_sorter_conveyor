/**
 *  @file TB6612FNG_Motor.cpp
 *  @brief Implements the TB6612FNG_Motor class for driving a DC motor using the TB6612FNG motor driver.
 *  @details This file contains the implementation of the TB6612FNG_Motor class, which provides functions for controlling a DC motor using the TB6612FNG motor driver. It includes functions for setting the motor duty cycle, enabling and disabling the motor, and setting the inverted compare value.
 *  @author Max Gardenswartz 
 *  @date May 18, 2024
 **/

#include "TB6612FNG_Motor.h"

/**
 * @brief Constructor for the TB6612FNG_Motor class.
 * @param minimum_duty_percent_to_prevent_twitch Duty cycles between this percent of the maximum will be set to 0.
 * @param IN1_pin_group The GPIO port for the IN1 pin (like GPIOA).
 * @param IN1_pin The pin number for the IN1 pin (like GPIO_PIN_0).
 * @param IN2_pin_group The GPIO port for the IN2 pin. (like GPIOA).
 * @param IN2_pin The pin number for the IN2 pim (like GPIO_PIN_0).
 * @param timer_handle A pointer to the timer handle for PWM generation (like &htim1).
 * @param timer_channel The channel number to be used with the timer (like TIM_CHANNEL_1).
 * @param CPU_clock_speed_MHz The CPU clock speed in MHz (like 96).
 * @param PWM_frequency_Hz The PWM frequency in Hz (20,000 to 100,000 allowed).
 */

TB6612FNG_Motor::TB6612FNG_Motor(
		GPIO_TypeDef* IN1_pin_group, uint16_t IN1_pin,
		GPIO_TypeDef* IN2_pin_group, uint16_t IN2_pin,
		TIM_HandleTypeDef* timer_handle, uint32_t timer_channel,
		uint32_t CPU_clock_speed_MHz, uint32_t PWM_frequency_Hz
	)

	// Note that all channels on the timer will have the same PWM frequency and prescalar

	: IN1_pin_group(IN1_pin_group), IN1_pin(IN1_pin),
	  IN2_pin_group(IN2_pin_group), IN2_pin(IN2_pin),
	  timer_handle(timer_handle), timer_channel(timer_channel),
	  CPU_clock_speed_MHz(CPU_clock_speed_MHz),
	  PWM_frequency_Hz(PWM_frequency_Hz)
{
	// Make sure the motor is off to start
	stop();

	// Make sure the PWM frequency is within bounds
	if(PWM_frequency_Hz < 20000)
	{
		// Too low. Human can hear this.
		PWM_frequency_Hz = 20000;
	}
	else if(PWM_frequency_Hz > 100000)
	{
		// Very high. May interfere with other tasks.
		PWM_frequency_Hz = 100000;
	}

	// Set the auto-reload value based on the CPU clock speed and the desired PWM frequency
	auto_reload_value =( CPU_clock_speed_MHz * 1000000 / PWM_frequency_Hz ) - 1;
	timer_handle->Init.AutoReloadPreload = auto_reload_value;

	// Start the PWM generation
	start();
}

/**
 * @brief Destructor for the TB6612FNG_Motor class.
 */
TB6612FNG_Motor::~TB6612FNG_Motor()
{
	stop();
}

/**
 * @brief Sets the duty cycle of the motor.
 * @param speed The duty cycle value, ranging from -100 to 100 by default.
 */
void TB6612FNG_Motor::set_duty(int16_t duty, int16_t maximum_value)
{
	// Raise the appropriate pins
	if(duty >= 0)
	{
		HAL_GPIO_WritePin(IN1_pin_group, IN1_pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(IN2_pin_group, IN2_pin, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(IN1_pin_group, IN1_pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(IN2_pin_group, IN2_pin, GPIO_PIN_SET);
	}

	uint16_t duty_magnitude;
	if(duty<0)
	{
		duty_magnitude = duty*-1;
	}
	else
	{
		duty_magnitude = duty;
	}

	// Check if duty cycle is within bounds
	if(duty_magnitude > maximum_value)
	{
		duty = maximum_value;
	}


	// Set the compare value
	uint16_t compare_value = duty_magnitude * auto_reload_value / maximum_value;
	__HAL_TIM_SET_COMPARE(timer_handle, timer_channel, compare_value);
}

/**
 * @brief starts the motor by starting PWM generation.
 */
void TB6612FNG_Motor::start()
{
	HAL_TIM_PWM_Start(timer_handle, timer_channel);
}

/**
 * @brief stops the motor by stopping PWM generation.
 */
void TB6612FNG_Motor::stop()
{	
	HAL_GPIO_WritePin(IN1_pin_group, IN1_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IN2_pin_group, IN2_pin, GPIO_PIN_RESET);
	HAL_TIM_PWM_Stop(timer_handle, timer_channel);
}