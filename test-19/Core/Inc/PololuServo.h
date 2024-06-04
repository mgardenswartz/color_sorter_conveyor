/**
 * @file PololuServo.h
 * @brief Header for PololuServo.cpp
 * @author Max Gardenswartz
 * @date Jun 3, 2024
 */

#ifndef SRC_POLOLUSERVO_H_
#define SRC_POLOLUSERVO_H_

#include "stm32f4xx_hal.h"

class PololuServo {
public:
	PololuServo(TIM_HandleTypeDef* timer_handle,
				uint32_t timer_channel,
				uint16_t PWM_period_us,
				uint32_t CPU_clock_speed_MHz,
				uint16_t maximum_angle_magnitude = 120,
				uint16_t full_left_pulse_width_us = 1000,
				uint16_t neutral_pulse_width_us = 1500,
				uint16_t full_right_pulse_width_us = 2000);
	void set_position(int16_t position_deg);
	virtual ~PololuServo();


	// Attributes
	TIM_HandleTypeDef* timer_handle;
	uint32_t timer_channel;
	uint16_t PWM_period_us;
	uint32_t CPU_clock_speed_MHz;
	uint16_t maximum_angle_magnitude;
	uint16_t full_left_pulse_width_us;
	uint16_t neutral_pulse_width_us;
	uint16_t full_right_pulse_width_us;

private:
	uint16_t autoreload_value = 50000;
};

#endif /* SRC_POLOLUSERVO_H_ */
