/**
 * @file ConveyorBelt.h
 * @brief Header for ConveyorBelt.cpp.
 * @author Max Gardenswartz
 * @date Jun 6, 2024
 */

#ifndef SRC_CONVEYORBELT_H_
#define SRC_CONVEYORBELT_H_

#include "PololuEncoder.h"
#include "stm32f4xx_hal.h"
#include "defines.h"

class ConveyorBelt {
public:
	ConveyorBelt(PololuEncoder* encoder,
			     float roller_diameter_mm);
	virtual ~ConveyorBelt();
	float get_position();
	void debug_message(UART_HandleTypeDef* uart_handle);

	// Attributes
	PololuEncoder* encoder;
	uint8_t roller_diameter_mm;

	// Other variables
	float linear_position;
};

#endif /* SRC_CONVEYORBELT_H_ */
