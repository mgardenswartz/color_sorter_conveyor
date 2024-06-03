/**
 * @file MotorControl.cpp
 * @brief Takes an encoder and a motor and runs a PID controller
 * @author Max Gardenswartz
 * @date Jun 2, 2024
 */

#include "MotorControl.h"

MotorControl::MotorControl(
						   TB6612FNG_Motor* motor,
						   PololuEncoder* encoder,
						   uint32_t control_frequency_Hz,
						   float K_P,
						   float K_I,
						   float K_D
		)
		: motor(motor),
		  encoder(encoder),
		  control_frequency_Hz(control_frequency_Hz),
		  K_P(K_P),
		  K_I(K_I),
		  K_D(K_D)
{
	reinitialize();
}

MotorControl::~MotorControl()
{
	motor->stop();
}


void MotorControl::debug_message(UART_HandleTypeDef* uart_handle)
{
	#define VALUE_WIDTH 5
	#define MESSAGE_LENGTH 100
	char my_message[MESSAGE_LENGTH] = "";
    int string_length = snprintf(my_message, MESSAGE_LENGTH, "Controller effort: %*i/%*i.\r\n",
                             VALUE_WIDTH, effort,
							 VALUE_WIDTH, saturation_limit);
    HAL_UART_Transmit(uart_handle, (uint8_t*)my_message, string_length, HAL_MAX_DELAY);
    encoder->debug_message(uart_handle);
}

void MotorControl::run(float setpoint)
{
	// Grab the latest data from the encoder.
	encoder->update();
	float process_value = encoder->get_speed(control_frequency_Hz);

	// Get the effort and send to motor
	get_effort_sat(setpoint, process_value);
	motor->set_duty(effort, saturation_limit);
}

int16_t MotorControl::get_effort_sat(
					float setpoint,
					float process_value
					)
{
	// Calculating error for P control
	error = process_value - setpoint; // RPM, a float

	// Calculating running error for I control
	running_error += error/((float)control_frequency_Hz); // RPM*s, float arithmetic cast to an int

	// Calculating error slope for D control
	error_slope = (error-last_error)*((float)control_frequency_Hz); // RPM/s, a float
	last_error = error;

	// Controller effort
    float P_effort = K_P*error;
    float I_effort = K_I*(float)running_error;
    float D_effort = K_D*error_slope;
    float effort_unsat = P_effort + I_effort + D_effort;

    // Saturation
	if (effort_unsat > (float)saturation_limit)
	{
		effort = saturation_limit;
	}
	else if (effort_unsat < -1*(float)saturation_limit)
	{
		effort = -saturation_limit;
	}
	else
	{
		effort = (int16_t)effort_unsat;
	}

	return effort;
}

void MotorControl::reinitialize()
{
	// Stop motor
	motor->set_duty(0);

	// Reset important stuff
	last_error = 0;
    error = 0;
    effort = 0;
    running_error = 0; // Most important

    // Grab the new saturation limit.
    saturation_limit = motor->timer_handle->Instance->ARR; // Assuming no prescalar is used.
}

void MotorControl::update_KP(float new_KP)
{
	reinitialize();
	K_P = new_KP;
}

void MotorControl::update_KI(float new_KI)
{
	reinitialize();
	K_I = new_KI;
}

void MotorControl::update_KD(float new_KD)
{
	reinitialize();
	K_D = new_KD;
}

void MotorControl::update_control_frequency(uint32_t new_control_frequency_Hz)
{
	reinitialize();
	control_frequency_Hz = new_control_frequency_Hz;
}

