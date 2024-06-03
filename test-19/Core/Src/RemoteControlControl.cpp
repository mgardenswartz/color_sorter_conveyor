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
RemoteControlControl::RemoteControlControl(
		RCChannel* rc_channel,
		MotorControl* motor_controller,
		uint16_t maximum_speed)
	: rc_channel(rc_channel),
	  motor_controller(motor_controller),
	  maximum_speed(maximum_speed)
{
//	// Reassign the magnitudes.
//	rc_channel->rescale_forward_magnitude = maximum_speed;
//	rc_channel->rescale_reverse_magnitude = maximum_speed;
}

/**
 * @brief Updates a TB6612FNG_Motor object based on the remote control channel value.
 * Run this function in your HAL_TIM_IC_CaptureCallback() function to update the motor as often as your RC sends new values.
 */
void RemoteControlControl::update_motor()
{
	// Read from the remote control channel
	int32_t value = rc_channel->value;

	// Rescale to maximum speed.
	if (value>=0)
	{
		setpoint = value*maximum_speed/(rc_channel->rescale_forward_magnitude);
	}
	else
	{
		setpoint = value*maximum_speed/(rc_channel->rescale_reverse_magnitude);
	}

	// Update the controller
	// motor_controller->run(setpoint);
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
	if (motor_controller != nullptr)
	{
		delete motor_controller;
		motor_controller = nullptr; // Optional: Set to nullptr to avoid dangling pointers
	}
}

