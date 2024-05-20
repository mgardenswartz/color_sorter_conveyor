/**
 * @file RCChannel.cpp
 * @brief Creates an RC channel object on the STM32F411CUE6 that can be used to read PWM signals from a remote control receiver.
 * 
 * This file contains the implementation of the RCChannel class. The RCChannel class provides functionality to read PWM signals from a remote control receiver and perform operations such as scaling and rescaling the values.
 * 
 * @author Max Gardenswartz
 * @date May 18, 2024
 */

#include "RCChannel.h"

/**
 * @brief Constructs an RCChannel object.
 * 
 * @note This constructor initializes the RCChannel object with the specified parameters. 
 * It also starts the first interrupt and sets the minuend depending on the timer instance. 
 * It calculates the scaling factor and initializes the forward and reverse ranges.
 * 
 * @param timer_handle Pointer to the TIM_HandleTypeDef structure for pin being read from.
 * @param timer_channel Timer channel number.
 * @param RC_signal_period_us Period of the RC signal in microseconds. 17.5 ms for the DumboRC X6B receiver.
 * @param CPU_clock_speed_MHz CPU clock speed in MHz. 96 MHz for the STM32F411CUE6.
 * @param full_reverse_us_width Width of the full reverse signal in microseconds.
 * @param neutral_us_width Width of the neutral signal in microseconds.
 * @param full_forward_us_width Width of the full forward signal in microseconds.
 * @param rescale_forward_magnitude Maximum value for rescaling the forward signal. Overridden if fed into RemoteControlControl.
 * @param rescale_reverse_magnitude Minimum value for rescaling the reverse signal. Overridden if fed into RemoteControlControl.
 */
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
	if (((timer_handle->Instance) == TIM2) or ((timer_handle->Instance) == TIM5))
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

/**
 * @brief Put this into HAL_TIM_IC_CaptureCallback() for the associated timer and channel.
 * 
 * This function is called when the signal on the RC reads a rising or falling edge. 
 * It calculates the width of the RC signal, rescales it based on the forward and 
 * reverse ranges, and caps the value if necessary.
 * 
 * @note This function assumes that the timer handle and channel have been properly initialized.
 * Please set the associated timer channel to input capture mode, direct. 
 * Leave the auto-reload value as large as possible.
 */
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

 	// Finally
 	first_capture = true;
 	}
}

/**
 * @brief Destructor for the RCChannel class.
 * 
 * This destructor stops the interrupts for the RCChannel object.
 */
RCChannel::~RCChannel() {
	// Stop interrupts
	HAL_TIM_IC_Stop_IT(timer_handle, timer_channel);
}

