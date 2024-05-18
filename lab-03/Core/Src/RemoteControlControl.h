/*
 * RemoteControlControl.h
 *
 *  Created on: May 15, 2024
 *      Author: Max
 */

#ifndef SRC_REMOTECONTROLCONTROL_H_
#define SRC_REMOTECONTROLCONTROL_H_

#include "stm32f4xx_hal.h"
#include "MotorDriver.h"
#include "RCChannel.h"

class RemoteControlControl {
public:
	RemoteControlControl(
			RCChannel* rc_channel,
			MotorDriver* motor,
			bool invert
			);
	void update();
	virtual ~RemoteControlControl();

	RCChannel* rc_channel;
	MotorDriver* motor;
	bool invert;
private:
	// Attributes

};

#endif /* SRC_REMOTECONTROLCONTROL_H_ */