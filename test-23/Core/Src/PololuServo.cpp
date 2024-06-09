/**
 * @file PololuServo.cpp
 * @brief Driver for a FEETECH FS90-FB Servo
 * @author Max Gardenswartz
 * @date Jun 3, 2024
 */

#include "PololuServo.h"

PololuServo::PololuServo(TIM_HandleTypeDef* timer_handle,
						uint32_t timer_channel,
						uint16_t PWM_period_us,
						uint32_t CPU_clock_speed_MHz,
						uint16_t maximum_angle_magnitude,
						uint16_t full_left_pulse_width_us,
						uint16_t neutral_pulse_width_us,
						uint16_t full_right_pulse_width_us)
						: timer_handle(timer_handle),
						  timer_channel(timer_channel),
						  PWM_period_us(PWM_period_us),
						  CPU_clock_speed_MHz(CPU_clock_speed_MHz),
						  maximum_angle_magnitude(maximum_angle_magnitude),
						  full_left_pulse_width_us(full_left_pulse_width_us),
						  neutral_pulse_width_us(neutral_pulse_width_us),
						  full_right_pulse_width_us(full_right_pulse_width_us)
{
	// Set correct PWM period.
	timer_handle->Instance->ARR = autoreload_value;
	timer_handle->Instance->PSC = (CPU_clock_speed_MHz * PWM_period_us)/autoreload_value;
	HAL_TIM_PWM_Start(timer_handle, timer_channel);

	// Home servo
	set_position(0);
}

void PololuServo::set_position(int16_t position_deg) // should be int8_t
{
	// Saturate requested position
	if (position_deg > maximum_angle_magnitude){
		position_deg = maximum_angle_magnitude;
	}
	else if (position_deg < -maximum_angle_magnitude)
	{
		position_deg = -maximum_angle_magnitude;
	}

	// Convert the postion requested from degrees to a pulse width in microseconds.
	float slope;
	if (position_deg>=0)
	{
		slope = ((float)full_right_pulse_width_us - (float)neutral_pulse_width_us)/(float)maximum_angle_magnitude;
	}
	else
	{
		slope = ((float)neutral_pulse_width_us - (float)full_left_pulse_width_us)/(float)maximum_angle_magnitude;
	}

	//	if (slope<0)
	//	{
	//		slope *= -1;
	//	}

	// Calculate compare value from pulse width
	uint16_t pulse_width = slope*position_deg + neutral_pulse_width_us;
	uint16_t compare_value = pulse_width*autoreload_value/PWM_period_us;
	__HAL_TIM_SET_COMPARE(timer_handle, timer_channel, compare_value);
}

PololuServo::~PololuServo() {
	// TODO Auto-generated destructor stub
}
