/*
 * RemoteControlControl.cpp
 *
 *  Created on: May 15, 2024
 *      Author: Max
 */

#include "RemoteControlControl.h"

RemoteControlControl::RemoteControlControl(
		RCChannel* rc_channel,
		MotorDriver* motor,
		bool invert)
		:
		rc_channel(rc_channel),
		motor(motor),
		invert(invert)
	{
		// Reassign the magnitudes for rescaling to prevent rounding error.
		rc_channel->rescale_forward_magnitude = motor->timer->Init.Period;
		rc_channel->rescale_reverse_magnitude = rc_channel->rescale_forward_magnitude;
	}

void RemoteControlControl::update()
{
	//
	// Read from the remote control channel
	int32_t value = rc_channel->value;

	// Set direction
	bool direction;
	if(value<0)
	{
		direction = true;
		value *= -1;
	}
	else
	{
		direction = false;
	}

	// Update the motor
	motor->set_inverted_compare_value(value, direction);

}
RemoteControlControl::~RemoteControlControl()
{
    // Release any dynamically allocated resources
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
