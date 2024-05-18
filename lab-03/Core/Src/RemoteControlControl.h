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
	RCChannel* rc_channel;
	MotorDriver* motor;
	bool invert;

	RemoteControlControl(
			RCChannel* rc_channel,
			MotorDriver* motor,
			bool invert = false
			);
	void update();
	virtual ~RemoteControlControl();


private:
	// Attributes

};

#endif /* SRC_REMOTECONTROLCONTROL_H_ */
