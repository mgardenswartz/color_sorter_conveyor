/**
 * @file ColoredBlock.cpp
 * @brief Colored block class to track blocks.
 * @details A C++ object abstraction for tracking a block as it moves along a conveyor belt.
 * @author Max Gardenswartz
 * @date Jun 6, 2024
 */

#include "ColoredBlock.h"

/**
 * @brief Constructor for the ColoredBlock class.
 * @param conveyor A pointer to the ConveyorBelt object used to track the position of the block.
 * @details Initializes a ColoredBlock object and sets its home position using the current position of the conveyor belt.
 */
ColoredBlock::ColoredBlock(
        ConveyorBelt* conveyor)
    : conveyor(conveyor)
{
    // Set home on instantiation
    set_home();
}

/**
 * @brief Sets the home position of the block.
 * @details This function captures the current position of the conveyor belt and sets it as the home position for the block.
 */
void ColoredBlock::set_home()
{
    home = conveyor->get_position();
}

/**
 * @brief Updates the current position of the block.
 * @return The updated position of the block relative to its home position.
 * @details This function calculates the block's position by subtracting the home position from the current conveyor belt position.
 */
float ColoredBlock::update_position()
{
    position = conveyor->get_position() - home;
    return position;
}

/**
 * @brief Updates the color status of the block.
 * @param new_color The new color status of the block.
 * @details This function updates the block's color attribute based on the provided parameter.
 */
void ColoredBlock::update_color(bool new_color)
{
    color = new_color;
}

/**
 * @brief Destructor for the ColoredBlock class.
 * @details This destructor cleans up any resources used by the ColoredBlock object.
 */
ColoredBlock::~ColoredBlock()
{
	// No action needed.
}
