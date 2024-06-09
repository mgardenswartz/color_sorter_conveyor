/**
 *  @file RemoteControlControl.h
 *  @brief Header file for RemoteControlControl class.
 *  @author Max Gardenswartz 
 *  @date May 18, 2024
 **/

#ifndef SRC_REMOTECONTROLCONTROL_H_
#define SRC_REMOTECONTROLCONTROL_H_

#include "stm32f4xx_hal.h"
#include "MotorControl.h"
#include "RCChannel.h"

class RemoteControlControl {
	public:

		// Attributes
		RCChannel* rc_channel;
		MotorControl* motor_controller;
		uint16_t maximum_speed;

		// Other

		float setpoint;
		RemoteControlControl(
				RCChannel* rc_channel,
				MotorControl* motor_controller,
				uint16_t maximum_speed
				);
		void update_motor();
		void debug_message(UART_HandleTypeDef* uart_handle);
		virtual ~RemoteControlControl();
	private:
};

#endif /* SRC_REMOTECONTROLCONTROL_H_ */