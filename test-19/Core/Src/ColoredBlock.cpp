/**
 * @file ColoredBlock.cpp
 * @brief Colored block class to track blocks.
 * @author Max Gardenswartz
 * @date Jun 6, 2024
 */

#include "ColoredBlock.h"

ColoredBlock::ColoredBlock(ConveyorBelt* conveyor)
{
	// TODO Auto-generated constructor stub
}

void ColoredBlock::update_position(float new_position)
{
	position = new_position;
}

void ColoredBlock::update_color(bool new_color)
{
	color = new_color;
}

ColoredBlock::~ColoredBlock()
{
}

