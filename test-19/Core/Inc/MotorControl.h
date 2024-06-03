/**
 * @file MotorControl.h
 * @brief Header for MotorControl.cpp
 * @author Max Gardenswartz
 * @date Jun 2, 2024
 */

#ifndef SRC_MOTORCONTROL_H_
#define SRC_MOTORCONTROL_H_

#include "stm32f4xx_hal.h"
#include "PololuEncoder.h"
#include "TB6612FNG_Motor.h"

class MotorControl {
public:
	MotorControl(
			   TB6612FNG_Motor* motor,
			   PololuEncoder* encoder,
			   uint32_t control_frequency_Hz,
			   float K_P,
			   float K_I = 0,
			   float K_D = 0
			   );
	virtual ~MotorControl();
	void reinitialize();
	void run(float setpoint);
	void debug_message(UART_HandleTypeDef* uart_handle);
	int16_t get_effort_sat(
			float setpoint,
			float process_value
			);
	void update_KP(float new_KP);
	void update_KI(float new_KI);
	void update_KD(float new_KD);
	void update_control_frequency(uint32_t new_control_frequency_Hz);

	// Attributes
	TB6612FNG_Motor* motor;
	PololuEncoder* encoder;
	uint32_t control_frequency_Hz;
	float K_P;
	float K_I;
	float K_D;

	// For between methods
	float last_error = 0;
    float error = 0;
    float running_error = 0;
    float error_slope;
    int16_t effort = 0;
    int16_t saturation_limit;
    float SP;
private:
};


//self.controlFrequency = controlFrequency
//self.error=0
//effort_sat=0
//running_error=0
//self.controlPeriod = 1/self.controlFrequency
#endif /* SRC_MOTORCONTROL_H_ */
