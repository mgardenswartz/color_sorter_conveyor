/**
 * @file MotorControl.cpp
 * @brief Takes an encoder and a motor and runs a PID controller
 * @author Max Gardenswartz
 * @date Jun 2, 2024
 */

#include "MotorControl.h"

/**
 * @brief Constructor for the MotorControl class.
 * @param motor A pointer to the TB6612FNG_Motor object used to control the motor.
 * @param encoder A pointer to the PololuEncoder object used for feedback.
 * @param control_frequency_Hz The control frequency in Hertz.
 * @param K_P The proportional gain for the PID controller.
 * @param K_I The integral gain for the PID controller.
 * @param K_D The derivative gain for the PID controller.
 * @details Initializes a MotorControl object with the given motor, encoder, control frequency, and PID gains. Calls the reinitialize() method to reset internal states.
 */
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

/**
 * @brief Destructor for the MotorControl class.
 * @details Stops the motor when the MotorControl object is destroyed.
 */
MotorControl::~MotorControl()
{
    motor->stop();
}

/**
 * @brief Sends a debug message with the motor control status over UART.
 * @param uart_handle A pointer to the UART_HandleTypeDef object for UART communication.
 * @details This function formats and transmits a message containing the setpoint, effort, error, and running error of the motor control.
 */
void MotorControl::debug_message(UART_HandleTypeDef* uart_handle)
{
    char my_message[MESSAGE_LENGTH] = "";
    int string_length = snprintf(my_message, MESSAGE_LENGTH, "SP %*.2f, EFF: %*i/%*i, ERR: %*.3f RPM, Intgtr: %*.3f \r\n",
                             VALUE_WIDTH, SP,
                             VALUE_WIDTH, effort,
                             VALUE_WIDTH, saturation_limit,
                             VALUE_WIDTH, error,
                             VALUE_WIDTH, running_error );
    HAL_UART_Transmit(uart_handle, (uint8_t*)my_message, string_length, HAL_MAX_DELAY);
    encoder->debug_message(uart_handle);
}

/**
 * @brief Runs the motor control algorithm with a given setpoint.
 * @param setpoint The desired motor speed in RPM.
 * @details This function updates the encoder, calculates the control effort, and sets the motor duty cycle accordingly.
 */
void MotorControl::run(float setpoint)
{
    // Store to the class
    SP = setpoint;

    // Grab the latest data from the encoder.
    encoder->update();
    float process_value = encoder->get_speed(control_frequency_Hz);

    // Get the effort and send to motor
    get_effort_sat(setpoint, process_value);
    motor->set_duty(effort, saturation_limit);
}

/**
 * @brief Calculates the control effort with saturation limits.
 * @param setpoint The desired motor speed in RPM.
 * @param process_value The current motor speed in RPM.
 * @return The control effort after applying saturation limits.
 * @details This function calculates the PID control effort and applies saturation limits to prevent excessive control signals.
 */
int16_t MotorControl::get_effort_sat(
                    float setpoint,
                    float process_value
                    )
{
    // Calculating error for P control
    error = setpoint - process_value; // RPM, a float

    // Calculating running error for I control
    running_error += error / control_frequency_Hz; // RPM*s, float arithmetic cast to an int

    // Calculating error slope for D control
    error_slope = (error - last_error) * ((float)control_frequency_Hz); // RPM/s, a float
    last_error = error;

    // Controller effort
    float P_effort = K_P * error;
    float I_effort = K_I * running_error;
    float D_effort = K_D * error_slope;
    float effort_unsat = P_effort + I_effort + D_effort;

    // Saturation
    if (effort_unsat > (float)saturation_limit)
    {
        effort = saturation_limit;
    }
    else if (effort_unsat < -1 * (float)saturation_limit)
    {
        effort = -saturation_limit;
    }
    else
    {
        effort = (int16_t)effort_unsat;
    }

    return effort;
}

/**
 * @brief Reinitializes the motor control state.
 * @details This function stops the motor, resets the PID error terms, and updates the saturation limit.
 */
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

/**
 * @brief Updates the proportional gain (K_P) of the PID controller.
 * @param new_KP The new proportional gain.
 * @details This function updates the proportional gain and reinitializes the controller.
 */
void MotorControl::update_KP(float new_KP)
{
    reinitialize();
    K_P = new_KP;
}

/**
 * @brief Updates the integral gain (K_I) of the PID controller.
 * @param new_KI The new integral gain.
 * @details This function updates the integral gain and reinitializes the controller.
 */
void MotorControl::update_KI(float new_KI)
{
    reinitialize();
    K_I = new_KI;
}

/**
 * @brief Updates the derivative gain (K_D) of the PID controller.
 * @param new_KD The new derivative gain.
 * @details This function updates the derivative gain and reinitializes the controller.
 */
void MotorControl::update_KD(float new_KD)
{
    reinitialize();
    K_D = new_KD;
}

/**
 * @brief Updates the control frequency of the PID controller.
 * @param new_control_frequency_Hz The new control frequency in Hertz.
 * @details This function updates the control frequency and reinitializes the controller.
 */
void MotorControl::update_control_frequency(uint32_t new_control_frequency_Hz)
{
    reinitialize();
    control_frequency_Hz = new_control_frequency_Hz;
}
