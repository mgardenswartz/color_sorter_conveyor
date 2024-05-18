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
	// Read from the remote control channel
	int32_t value = rc_channel->value;

	if(value<0)
	{
		bool direction = true;
	}
	else
	{
		bool direction = false;
	}

	// Update the motor
	motor->set_compare_value(value, direction);

}
RemoteControlControl::~RemoteControlControl()
{
	// TODO Auto-generated destructor stub
}

