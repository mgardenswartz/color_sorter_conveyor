/**
 * @file RemoteControlControl.cpp
 * @brief Takes in a remote control channel and a motor driver and updates the motor driver based on the remote control channel value.
 * @author Max Gardenswartz 
 * @date May 19, 2024
 */

#include "RemoteControlControl.h"

/**
 * @brief Constructs a RemoteControlControl object with the given remote control channel and motor driver.
 * @param rc_channel Pointer to the RCChannel object.
 * @param motor Pointer to a TB6612FNG_Motor object.
 */
RemoteControlControl::RemoteControlControl(RCChannel* rc_channel, TB6612FNG_Motor* motor)
	: rc_channel(rc_channel), motor(motor)
{
	// Reassign the magnitudes for rescaling to prevent rounding error.
	new_maximum_value = motor->timer->Init.Period;
	rc_channel->rescale_forward_magnitude = new_maximum_value;
	rc_channel->rescale_reverse_magnitude = new_maximum_value;
}

/**
 * @brief Updates a TB6612FNG_Motor object based on the remote control channel value.
 * Run this function in your HAL_TIM_IC_CaptureCallback() function to update the motor as often as your RC sends new values.
 */
void RemoteControlControl::update_motor()
{
	// Read from the remote control channel
	int32_t value = rc_channel->value;

	// Update the motor
	motor->set_speed(value, new_maximum_value);
}

/**
 * @brief Destructs the RemoteControlControl object and releases any dynamically allocated resources.
 */
RemoteControlControl::~RemoteControlControl()
{
	// Deallocate memory for rc_channel if it was dynamically allocated
	if (rc_channel != nullptr)
	{
		delete rc_channel;
		rc_channel = nullptr; // Optional: Set to nullptr to avoid dangling pointers
	}

	// Deallocate memory for motor if it was dynamically allocated
	if (motor != nullptr)
	{
		delete motor;
		motor = nullptr; // Optional: Set to nullptr to avoid dangling pointers
	}
}
