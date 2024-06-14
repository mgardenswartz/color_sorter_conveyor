/**
 * @file ConveyorBelt.cpp
 * @brief Conveyorr belt object to track objects.
 * @author Max Gardenswartz
 * @date Jun 6, 2024
 */

#include "ConveyorBelt.h"

/**
 * @brief Constructor for the ConveyorBelt class.
 * @param encoder A pointer to the PololuEncoder object used to track the position.
 * @param roller_diameter_mm The diameter of the conveyor belt roller in millimeters.
 * @details Initializes a ConveyorBelt object with the given encoder and roller diameter.
 */
ConveyorBelt::ConveyorBelt(PololuEncoder* encoder, float roller_diameter_mm)
    : encoder(encoder), roller_diameter_mm(roller_diameter_mm)
{
}

/**
 * @brief Gets the current position of the conveyor belt.
 * @return The current linear position of the conveyor belt in millimeters.
 * @details This function reads the encoder's position in ticks, converts it to revolutions, and then to a linear position.
 */
float ConveyorBelt::get_position()
{
    // Get the latest encoder position in ticks
    // encoder->update();
    int32_t encoder_count = encoder->get_position();

    // Convert that to a number of revolutions, then a linear position
    float revolutions = encoder_count / (encoder->gear_ratio * (float)encoder->CPR);
    linear_position = revolutions * 3.14159 * 2 * roller_diameter_mm / 2;
    return linear_position;
}

/**
 * @brief Sends a debug message with the conveyor belt's position over UART.
 * @param uart_handle A pointer to the UART_HandleTypeDef object for UART communication.
 * @details This function formats and transmits a message containing the current position of the conveyor belt.
 */
void ConveyorBelt::debug_message(UART_HandleTypeDef* uart_handle)
{
    char my_message[MESSAGE_LENGTH] = "";
    int string_length = snprintf(my_message, MESSAGE_LENGTH, "Conveyor Belt Position: %*.3f mm.\r\n",
                                 VALUE_WIDTH, linear_position);
    HAL_UART_Transmit(uart_handle, (uint8_t*)my_message, string_length, HAL_MAX_DELAY);
}

/**
 * @brief Destructor for the ConveyorBelt class.
 * @details Cleans up any resources used by the ConveyorBelt object. Currently, no action is needed.
 */
ConveyorBelt::~ConveyorBelt()
{
    // No action needed.
}

