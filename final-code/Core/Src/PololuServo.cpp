/**
 * @file PololuServo.cpp
 * @brief Driver for a FEETECH FS90-FB Servo
 * @author Max Gardenswartz
 * @date Jun 3, 2024
 */

#include "PololuServo.h"

/**
 * @brief Constructor for the PololuServo class.
 * @param timer_handle A pointer to the TIM_HandleTypeDef object used for timer operations.
 * @param timer_channel The timer channel used for PWM output.
 * @param PWM_period_us The PWM period in microseconds.
 * @param CPU_clock_speed_MHz The CPU clock speed in MHz.
 * @param closed_loop_mode Boolean indicating whether to use closed-loop control mode.
 * @param maximum_angle_magnitude The maximum angle magnitude in degrees.
 * @param full_left_pulse_width_us The pulse width corresponding to the full left position in microseconds.
 * @param neutral_pulse_width_us The pulse width corresponding to the neutral position in microseconds.
 * @param full_right_pulse_width_us The pulse width corresponding to the full right position in microseconds.
 * @details Initializes a PololuServo object and sets the PWM period and prescaler. The servo is homed to the neutral position.
 */
PololuServo::PololuServo(TIM_HandleTypeDef* timer_handle,
                         uint32_t timer_channel,
                         uint16_t PWM_period_us,
                         uint32_t CPU_clock_speed_MHz,
                         bool closed_loop_mode,
                         uint16_t maximum_angle_magnitude,
                         uint16_t full_left_pulse_width_us,
                         uint16_t neutral_pulse_width_us,
                         uint16_t full_right_pulse_width_us)
    : timer_handle(timer_handle),
      timer_channel(timer_channel),
      PWM_period_us(PWM_period_us),
      CPU_clock_speed_MHz(CPU_clock_speed_MHz),
      closed_loop_mode(closed_loop_mode),
      maximum_angle_magnitude(maximum_angle_magnitude),
      full_left_pulse_width_us(full_left_pulse_width_us),
      neutral_pulse_width_us(neutral_pulse_width_us),
      full_right_pulse_width_us(full_right_pulse_width_us)
{
    // Set correct PWM period.
    timer_handle->Instance->ARR = autoreload_value;
    timer_handle->Instance->PSC = (CPU_clock_speed_MHz * PWM_period_us) / autoreload_value;
    HAL_TIM_PWM_Start(timer_handle, timer_channel);

    // Home servo
    set_position(0);
}

/**
 * @brief Sets the position of the servo.
 * @param position_deg The desired position in degrees.
 * @details This function sets the servo to the specified position in degrees. It converts the degree value to the corresponding PWM pulse width and updates the compare value of the timer.
 */
void PololuServo::set_position(int16_t position_deg)
{
    // Saturate requested position
    if (position_deg > maximum_angle_magnitude){
        position_deg = maximum_angle_magnitude;
    }
    else if (position_deg < -maximum_angle_magnitude)
    {
        position_deg = -maximum_angle_magnitude;
    }

    // Convert the position requested from degrees to a pulse width in microseconds.
    float slope;
    if (position_deg >= 0)
    {
        slope = ((float)full_right_pulse_width_us - (float)neutral_pulse_width_us) / (float)maximum_angle_magnitude;
    }
    else
    {
        slope = ((float)neutral_pulse_width_us - (float)full_left_pulse_width_us) / (float)maximum_angle_magnitude;
    }

    // Calculate compare value from pulse width
    uint16_t pulse_width = slope * position_deg + neutral_pulse_width_us;
    uint16_t compare_value = pulse_width * autoreload_value / PWM_period_us;
    __HAL_TIM_SET_COMPARE(timer_handle, timer_channel, compare_value);
}

/**
 * @brief Destructor for the PololuServo class.
 * @details Stops the PWM output and performs any necessary cleanup when the PololuServo object is destroyed.
 */
PololuServo::~PololuServo()
{
    // TODO Auto-generated destructor stub
}
