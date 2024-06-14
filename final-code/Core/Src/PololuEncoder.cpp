/**
 * @file PololuEncoder.cpp
 * @brief Driver for a Pololu two-channel gearmotor encoder
 * @author Max Gardenswartz
 * @date Jun 2, 2024
 */

#include "PololuEncoder.h"

/**
 * @brief Constructor for the PololuEncoder class.
 * @param timer_handle A pointer to the TIM_HandleTypeDef object used for timer operations.
 * @param CPR Counts per revolution of the encoder.
 * @param gear_ratio The gear ratio of the encoder setup.
 * @param timer_bit_size The bit size of the timer (e.g., 16 or 32 bits).
 * @param interrupt_mode Boolean indicating whether to use interrupt mode.
 * @details Initializes a PololuEncoder object and starts the encoder using either interrupt or non-interrupt mode. Also sets up the timer count limits based on the bit size.
 */
PololuEncoder::PololuEncoder(
        TIM_HandleTypeDef* timer_handle,
        int16_t CPR,
        float gear_ratio,
        uint8_t timer_bit_size,
        bool interrupt_mode
    )
    : timer_handle(timer_handle),
      CPR(CPR),
      gear_ratio(gear_ratio),
      timer_bit_size(timer_bit_size),
      interrupt_mode(interrupt_mode)
{
    // Start encoder
    if (interrupt_mode)
    {
        HAL_TIM_Encoder_Start_IT(timer_handle, TIM_CHANNEL_ALL);
    }
    else
    {
        HAL_TIM_Encoder_Start(timer_handle, TIM_CHANNEL_ALL);
    }

    // Number of bits for the select timer
    if (timer_bit_size == 32)
    {
        max_count = 0xffffffff;
    }
    else
    {
        max_count = 0xffff;
    }

    // Set pertinent values for later.
    half_max_count = max_count / 2;
    neg_half_max_count = -half_max_count;
}

/**
 * @brief Updates the encoder count and processes overflow corrections.
 * @details This function reads the current encoder count, calculates the delta since the last update, and adjusts for timer overflow. It then updates the position based on the delta.
 */
void PololuEncoder::update()
{
    // Read encoder
    if (!interrupt_mode)
    {
        count = __HAL_TIM_GET_COUNTER(timer_handle);
    }
    delta = (int32_t) (count - last_count);
    last_count = count;

    // Fix delta from timer overflow
    if (delta > half_max_count)
    {
        // We've overflowed in the negative direction.
        delta -= max_count + 1;
    }
    else if (delta < neg_half_max_count)
    {
        // We've overflowed in the positive direction.
        delta += max_count + 1;
    }

    // Finish up
    position += delta;
}

/**
 * @brief Resets the encoder position to zero.
 * @details This function sets the internal position count to zero.
 */
void PololuEncoder::zero()
{
    position = 0;
}

/**
 * @brief Gets the current position of the encoder.
 * @return The current position as an integer count.
 * @details This function returns the current position count of the encoder.
 */
int32_t PololuEncoder::get_position()
{
    return position;
}

/**
 * @brief Gets the change in position (delta) since the last update.
 * @return The change in position as an integer count.
 * @details This function returns the delta count since the last update.
 */
int16_t PololuEncoder::get_delta()
{
    return delta;
}

/**
 * @brief Calculates and returns the speed of the encoder in RPM.
 * @param control_frequency_Hz The control frequency in Hertz.
 * @return The speed in RPM.
 * @details This function calculates the speed of the encoder in RPM based on the delta count and control frequency.
 */
float PololuEncoder::get_speed(uint32_t control_frequency_Hz)
{
    float numerator = delta * (float)control_frequency_Hz * 60;
    float denominator = (float)CPR * gear_ratio;
    speed = numerator / denominator; // RPM
    return speed;
}

/**
 * @brief Sends a debug message with the encoder status over UART.
 * @param uart_handle A pointer to the UART_HandleTypeDef object for UART communication.
 * @details This function formats and transmits a message containing the current count, position, delta, and speed of the encoder.
 */
void PololuEncoder::debug_message(UART_HandleTypeDef* uart_handle)
{
    char my_message[MESSAGE_LENGTH] = "";
    int string_length = snprintf(my_message, MESSAGE_LENGTH, "Encoder cnt: %*i, pos: %*li, delta: %*i, speed: %*.3f RPM.\r\n",
                             VALUE_WIDTH, count,
                             VALUE_WIDTH, position,
                             VALUE_WIDTH, delta,
                             VALUE_WIDTH + 3, speed);
    HAL_UART_Transmit(uart_handle, (uint8_t*)my_message, string_length, HAL_MAX_DELAY);
}

/**
 * @brief Destructor for the PololuEncoder class.
 * @details Stops the encoder when the PololuEncoder object is destroyed.
 */
PololuEncoder::~PololuEncoder()
{
    // Stop encoder
    if (interrupt_mode)
    {
        HAL_TIM_Encoder_Stop_IT(timer_handle, TIM_CHANNEL_ALL);
    }
    else
    {
        HAL_TIM_Encoder_Stop(timer_handle, TIM_CHANNEL_ALL);
    }
}
