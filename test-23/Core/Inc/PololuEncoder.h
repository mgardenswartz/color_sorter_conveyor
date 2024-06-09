/**
 * @file PololuEncoder.h
 * @brief Header for PololuEncoder.cpp
 * @author Max Gardenswartz
 * @date Jun 2, 2024
 */

#ifndef SRC_POLOLUENCODER_H_
#define SRC_POLOLUENCODER_H_

#include "stm32f4xx_hal.h"
#include "defines.h"
#include <stdio.h>

class PololuEncoder {
public:
	PololuEncoder(
			TIM_HandleTypeDef* timer_handle,
			int16_t CPR,
			float gear_ratio = 1,
			uint8_t timer_bit_size = 16,
			bool interrupt_mode = false
			);
	virtual ~PololuEncoder();
	void update();
	void debug_message(UART_HandleTypeDef* uart_handle);
	void zero();
	int32_t get_position();
	int16_t get_delta();
	float get_speed(uint32_t control_frequency_Hz);

	// Init
	TIM_HandleTypeDef* timer_handle;
	int16_t CPR;
	float gear_ratio;
	uint8_t timer_bit_size;
	bool interrupt_mode;

	// Attributes
	int16_t delta = 0;
	uint16_t last_count = 0;
	uint16_t count = 0;
	int32_t position = 0;
	float speed = 0;

private:
	// Between methods
	uint32_t max_count;
	int32_t half_max_count;
	int32_t neg_half_max_count;
};

#endif /* SRC_POLOLUENCODER_H_ */
