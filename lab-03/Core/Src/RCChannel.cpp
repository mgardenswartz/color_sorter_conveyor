/*
 * RCChannel.cpp
 *
 *  Created on: May 15, 2024
 *      Author: Max
 */

#include "RCChannel.h"

RCChannel::RCChannel(
    TIM_HandleTypeDef* timer_handle,
    uint32_t timer_channel,
    int32_t RC_signal_period_us,
    uint32_t CPU_clock_speed_MHz,
    int32_t full_reverse_us_width,
    int32_t neutral_us_width,
    int32_t full_forward_us_width,
    int32_t rescale_forward_magnitude,
	int32_t rescale_reverse_magnitude)
	:
    timer_handle(timer_handle),
    timer_channel(timer_channel),
    RC_signal_period_us(RC_signal_period_us),
    CPU_clock_speed_MHz(CPU_clock_speed_MHz),
    full_reverse_us_width(full_reverse_us_width),
    neutral_us_width(neutral_us_width),
    full_forward_us_width(full_forward_us_width),
    rescale_forward_magnitude(rescale_forward_magnitude),
	rescale_reverse_magnitude(rescale_reverse_magnitude)
{
	// Critical. Starts the first interrupt.
	HAL_TIM_IC_Start_IT(timer_handle, timer_channel);

	// Set minuend depending on whether it's a 16-bit or 32-bit timer.
	// This is specific to the STM32F411CUE6!!
	if(( (timer_handle->Instance) == TIM2 ) or ( (timer_handle->Instance) == TIM5 ))
	{
		minuend = 0xffffffff;
	}
	else
	{
		minuend = 0xffff;
	}

	// For later scaling
	uint32_t prescalar = timer_handle->Instance->PSC + 1;
	float refClock = CPU_clock_speed_MHz/prescalar;
	mFactor = 1000000/refClock;

	// Needed later
	forward_range = full_forward_us_width - neutral_us_width;
	reverse_range = neutral_us_width - full_reverse_us_width;

}


void RCChannel::callback(){
	if(first_capture)
 	{
 		first_value = HAL_TIM_ReadCapturedValue(timer_handle, timer_channel);
 		first_capture = false;
 	}
 	else
 	{
 		second_value = HAL_TIM_ReadCapturedValue(timer_handle, timer_channel);

 		if(first_value > second_value)
 		{
 			difference = minuend - first_value + second_value + 1;
 		}
 		else
 		{
 			difference = second_value - first_value;
 		}

 		us_width = difference*mFactor;

		// Fix width
		if(us_width > RC_signal_period_us/2)
		{
			us_width = RC_signal_period_us - us_width;
		}

 		// Rescale
		int32_t centered_value = us_width - neutral_us_width;

		// Forward case
 		if(centered_value >= 0)
 		{
 			value = centered_value*rescale_forward_magnitude/forward_range;

 			// Cap
 	 		if(value>rescale_forward_magnitude)
 	 		{
 	 			value = rescale_forward_magnitude;
 	 		}
 		}

 		// Reverse case
 		else
		{
			value =  centered_value*rescale_reverse_magnitude/reverse_range;

			// Cap
	 		if(value<-rescale_reverse_magnitude)
	 		{
	 			value = -rescale_reverse_magnitude;
	 		}

		}

 		// Make sure it never exceeds the magnitude.

 	// Finally
 	first_capture = true;
 	}
}

RCChannel::~RCChannel() {
	// TODO Auto-generated destructor stub
}

