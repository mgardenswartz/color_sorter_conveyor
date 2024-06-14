/**
 *  @file TB6612FNG_Motor.cpp
 *  @brief Implements the TB6612FNG_Motor class for driving a DC motor using the TB6612FNG motor driver.
 *  @details This file contains the implementation of the TB6612FNG_Motor class, which provides functions for controlling a DC motor using the TB6612FNG motor driver. It includes functions for setting the motor duty cycle, enabling and disabling the motor, and setting the inverted compare value.
 *  @author Max Gardenswartz 
 *  @date May 18, 2024
 **/

#include "TB6612FNG_Motor.h"

/**
 * @brief Constructs a TB6612FNG_Motor object with the given parameters.
 * @param IN1_pin_group GPIO port for the IN1 pin.
 * @param IN1_pin GPIO pin number for the IN1 pin.
 * @param IN2_pin_group GPIO port for the IN2 pin.
 * @param IN2_pin GPIO pin number for the IN2 pin.
 * @param timer_handle Pointer to the timer handle used for PWM.
 * @param timer_channel Timer channel used for PWM.
 * @param CPU_clock_speed_MHz CPU clock speed in MHz.
 * @param PWM_frequency_Hz Desired PWM frequency in Hz.
 * @details Initializes the motor driver, sets up PWM parameters, and ensures the motor is initially stopped.
 */
TB6612FNG_Motor::TB6612FNG_Motor(
    GPIO_TypeDef* IN1_pin_group, uint16_t IN1_pin,
    GPIO_TypeDef* IN2_pin_group, uint16_t IN2_pin,
    TIM_HandleTypeDef* timer_handle, uint32_t timer_channel,
    uint32_t CPU_clock_speed_MHz, uint32_t PWM_frequency_Hz
)
    : IN1_pin_group(IN1_pin_group), IN1_pin(IN1_pin),
      IN2_pin_group(IN2_pin_group), IN2_pin(IN2_pin),
      timer_handle(timer_handle), timer_channel(timer_channel),
      CPU_clock_speed_MHz(CPU_clock_speed_MHz),
      PWM_frequency_Hz(PWM_frequency_Hz)
{
    // Make sure the motor is off to start
    stop();

    // Ensure the PWM frequency is within bounds
    if (PWM_frequency_Hz < 20000)
    {
        // Too low. Humans can hear this.
        PWM_frequency_Hz = 20000;
    }
    else if (PWM_frequency_Hz > 100000)
    {
        // Very high. May interfere with other tasks.
        PWM_frequency_Hz = 100000;
    }

    // Set the auto-reload value based on the CPU clock speed and the desired PWM frequency
    auto_reload_value = (CPU_clock_speed_MHz * 1000000 / PWM_frequency_Hz) - 1;
    timer_handle->Instance->ARR = auto_reload_value;

    // Start the PWM generation
    start();
}

/**
 * @brief Destructor for the TB6612FNG_Motor class.
 * @details Ensures the motor is stopped when the object is destroyed.
 */
TB6612FNG_Motor::~TB6612FNG_Motor()
{
    stop();
}

/**
 * @brief Sets the duty cycle of the motor.
 * @param duty The duty cycle value, ranging from -100 to 100 by default.
 * @param maximum_value The maximum value for the duty cycle.
 * @details Controls the motor speed and direction by setting the appropriate PWM duty cycle.
 */
void TB6612FNG_Motor::set_duty(int16_t duty, int16_t maximum_value)
{
    // Raise the appropriate pins
    if (duty >= 0)
    {
        HAL_GPIO_WritePin(IN1_pin_group, IN1_pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(IN2_pin_group, IN2_pin, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(IN1_pin_group, IN1_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(IN2_pin_group, IN2_pin, GPIO_PIN_SET);
    }

    uint16_t duty_magnitude = (duty < 0) ? -duty : duty;

    // Check if duty cycle is within bounds
    if (duty_magnitude > maximum_value)
    {
        duty_magnitude = maximum_value;
    }

    // Set the compare value
    uint16_t compare_value = duty_magnitude * auto_reload_value / maximum_value;
    __HAL_TIM_SET_COMPARE(timer_handle, timer_channel, compare_value);
}

/**
 * @brief Starts the motor by starting PWM generation.
 * @details Enables PWM signals to drive the motor.
 */
void TB6612FNG_Motor::start()
{
    HAL_TIM_PWM_Start(timer_handle, timer_channel);
}

/**
 * @brief Stops the motor by stopping PWM generation.
 * @details Disables PWM signals and stops the motor by setting control pins to low.
 */
void TB6612FNG_Motor::stop()
{
    HAL_GPIO_WritePin(IN1_pin_group, IN1_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN2_pin_group, IN2_pin, GPIO_PIN_RESET);
    HAL_TIM_PWM_Stop(timer_handle, timer_channel);
}
