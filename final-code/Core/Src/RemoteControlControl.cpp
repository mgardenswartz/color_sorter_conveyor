/**
 * @file RemoteControlControl.cpp
 * @brief Takes in a remote control channel and a motor driver and updates the motor driver based on the remote control channel value.
 * @author REDACTED
 * @date May 19, 2024
 */

#include "RemoteControlControl.h"

/**
 * @brief Constructs a RemoteControlControl object with the given remote control channel and motor driver.
 * @param rc_channel Pointer to the RCChannel object.
 * @param motor_controller Pointer to a MotorControl object.
 * @param maximum_speed The maximum speed to scale the RC channel value to.
 * @details Initializes the RemoteControlControl object with pointers to an RC channel and a motor controller, as well as a maximum speed for scaling.
 */
RemoteControlControl::RemoteControlControl(
    RCChannel* rc_channel,
    MotorControl* motor_controller,
    uint16_t maximum_speed)
    : rc_channel(rc_channel),
      motor_controller(motor_controller),
      maximum_speed(maximum_speed)
{
    // Initialization code here, if needed.
}

/**
 * @brief Updates the motor controller based on the remote control channel value.
 * @details This function should be called in the HAL_TIM_IC_CaptureCallback() function to update the motor as frequently as the RC sends new values.
 */
void RemoteControlControl::update_motor()
{
    // Read from the remote control channel
    int32_t value = rc_channel->value;

    // Rescale to maximum speed.
    if (value >= 0)
    {
        setpoint = (float)value * (float)maximum_speed / (float)rc_channel->rescale_forward_magnitude;
    }
    else
    {
        setpoint = (float)value * (float)maximum_speed / (float)rc_channel->rescale_reverse_magnitude;
    }

    // Update the controller
    motor_controller->run(setpoint);
}

/**
 * @brief Sends a debug message with the current throttle setpoint and RC channel value.
 * @param uart_handle Pointer to the UART handle used for communication.
 * @details This function transmits a formatted debug message containing the current throttle setpoint and the RC channel value over UART.
 */
void RemoteControlControl::debug_message(UART_HandleTypeDef* uart_handle)
{
    char my_message[100] = "";
    int string_length = snprintf(my_message, 100, "Throttle: %*.3f RPM (%*li)\r\n",
                                 10, setpoint,
                                 10, (long signed int)rc_channel->value);
    HAL_UART_Transmit(uart_handle, (uint8_t*)my_message, string_length, HAL_MAX_DELAY);
}

/**
 * @brief Destructs the RemoteControlControl object and releases any dynamically allocated resources.
 * @details This destructor ensures that any dynamically allocated memory for the RC channel and motor controller is properly released to prevent memory leaks.
 */
RemoteControlControl::~RemoteControlControl()
{
    // Deallocate memory for rc_channel if it was dynamically allocated
    if (rc_channel != nullptr)
    {
        delete rc_channel;
        rc_channel = nullptr; // Optional: Set to nullptr to avoid dangling pointers
    }

    // Deallocate memory for motor_controller if it was dynamically allocated
    if (motor_controller != nullptr)
    {
        delete motor_controller;
        motor_controller = nullptr; // Optional: Set to nullptr to avoid dangling pointers
    }
}
