/**
 * @file PololuEncoder.cpp
 * @brief Driver for a Pololu two-channel gearmotor encoder
 * @author Max Gardenswartz
 * @date Jun 2, 2024
 */

#include "PololuEncoder.h"

PololuEncoder::PololuEncoder(
		TIM_HandleTypeDef* timer_handle,
		int16_t CPR,
		float gear_ratio,
		uint8_t timer_bit_size,
		bool interrupt_mode
		)
		: timer_handle(timer_handle),
		  CPR(CPR),
		  gear_ratio(gear_ratio),
		  timer_bit_size(timer_bit_size),
		  interrupt_mode(interrupt_mode)
{
	// Start encoder
	if (interrupt_mode)
	{
		HAL_TIM_Encoder_Start_IT(timer_handle, TIM_CHANNEL_ALL);
	}
	else
	{
		HAL_TIM_Encoder_Start(timer_handle, TIM_CHANNEL_ALL);
	}

	// Number of bits for the select timer
	if (timer_bit_size == 32)
	{
		max_count = 0xffffffff;
	}
	else
	{
		max_count = 0xffff;
	}
	half_max_count = max_count/2;
	neg_half_max_count = -half_max_count;
}

void PololuEncoder::update()
{
	// Read encoder
	if (not interrupt_mode)
	{
		count = __HAL_TIM_GET_COUNTER(timer_handle);
	}
	delta = (int32_t) (count-last_count);
	last_count = count;

	// Fix delta from timer overflow
	if (delta > half_max_count)
	{
		// We've overflowed in the negative direction.
		delta -= max_count + 1;
	}
	else if (delta < neg_half_max_count)
	{
	    // We've overflowed in the positive direciton.
		delta += max_count + 1;
	}

	// Finish up
	position += delta;
}

void PololuEncoder::zero()
{
	position = 0;
}

int32_t PololuEncoder::get_position()
{
	return position;
}

int16_t PololuEncoder::get_delta()
{
	return delta;
}

float PololuEncoder::get_speed(uint32_t control_frequency_Hz)
{
	float numerator = delta*(float)control_frequency_Hz*60;
	float denominator = (float)CPR*gear_ratio;
    speed = numerator/denominator; //RPM
    return speed;
}

void PololuEncoder::debug_message(UART_HandleTypeDef* uart_handle)
{
	#define VALUE_WIDTH 5
	#define MESSAGE_LENGTH 100
	char my_message[MESSAGE_LENGTH] = "";
    int string_length = snprintf(my_message, MESSAGE_LENGTH, "Encoder cnt: %*i, pos: %*li, delta: %*i, speed: %*.3f RPM.\r\n",
                             VALUE_WIDTH, count,
                             VALUE_WIDTH, position,
                             VALUE_WIDTH, delta,
							 VALUE_WIDTH+3, speed);
    HAL_UART_Transmit(uart_handle, (uint8_t*)my_message, string_length, HAL_MAX_DELAY);
}

PololuEncoder::~PololuEncoder()
{
	// TODO Auto-generated destructor stub
}

