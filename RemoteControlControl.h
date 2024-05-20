/**
 *  @file RemoteControlControl.h
 *  @brief Header file for RemoteControlControl class.
 *  @author Max Gardenswartz 
 *  @date May 18, 2024
 **/

#ifndef SRC_REMOTECONTROLCONTROL_H_
#define SRC_REMOTECONTROLCONTROL_H_

#include "stm32f4xx_hal.h"
#include "TB6612FNG_Motor.h"
#include "RCChannel.h"

class RemoteControlControl {
	public:
		RCChannel* rc_channel;
		TB6612FNG_Motor* motor;

		RemoteControlControl(
				RCChannel* rc_channel,
				TB6612FNG_Motor* motor
				);
		void update_motor();
		virtual ~RemoteControlControl();
	private:
		uint32_t new_maximum_value = 100; 
};

#endif /* SRC_REMOTECONTROLCONTROL_H_ */
