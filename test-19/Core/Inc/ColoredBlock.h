/**
 * @file ColoredBlock.h
 * @brief Header for ColoredBlock.cpp.
 * @author Max Gardenswartz
 * @date Jun 6, 2024
 */

#ifndef SRC_COLOREDBLOCK_H_
#define SRC_COLOREDBLOCK_H_

#include "ConveyorBelt.h"

class ColoredBlock {
public:
	ColoredBlock(ConveyorBelt* conveyor);
	void update_position(float new_position);
	void update_color(bool new_color);
	virtual ~ColoredBlock();

	// Values
	float position = 0;
	bool color; // 1 is red. 0 is blue.

	// Attributes
};

#endif /* SRC_COLOREDBLOCK_H_ */
