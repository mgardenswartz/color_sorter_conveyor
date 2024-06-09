/**
 * @file ConveyorBelt.cpp
 * @brief Conveyorr belt object to track objects.
 * @author Max Gardenswartz
 * @date Jun 6, 2024
 */

#include "ConveyorBelt.h"

ConveyorBelt::ConveyorBelt(PololuEncoder* encoder,
						   uint8_t roller_diameter_mm)
							: encoder(encoder),
							  roller_diameter_mm(roller_diameter_mm)
{

}

float ConveyorBelt::get_position()
{
	// Get the latest encoder position in ticks
	encoder->update();
	int32_t encoder_count = encoder->get_position();

	// Convert that to a number of revolutions, then a linear position
	float revolutions = encoder_count/(encoder->gear_ratio * (float)encoder->CPR);
	linear_position = revolutions*roller_diameter_mm/2;
	return linear_position;
}

void ConveyorBelt::debug_message(UART_HandleTypeDef* uart_handle)
{
	char my_message[MESSAGE_LENGTH] = "";
    int string_length = snprintf(my_message, MESSAGE_LENGTH, "Conveyor Belt Position: %*.3f mm.\r\n",
                             VALUE_WIDTH, linear_position);
    HAL_UART_Transmit(uart_handle, (uint8_t*)my_message, string_length, HAL_MAX_DELAY);
}

ConveyorBelt::~ConveyorBelt()
{
	// Nothing needed
}
