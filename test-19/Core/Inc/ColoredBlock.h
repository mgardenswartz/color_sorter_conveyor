/**
 * @file ColoredBlock.h
 * @brief Header for ColoredBlock.cpp.
 * @author Max Gardenswartz
 * @date Jun 6, 2024
 */

#ifndef SRC_COLOREDBLOCK_H_
#define SRC_COLOREDBLOCK_H_

class ColoredBlock {
public:
	ColoredBlock();
	void update_position();
	virtual ~ColoredBlock();

	// Values
	float position = 0;
	char color[10] = "";

	// Attributes
};

#endif /* SRC_COLOREDBLOCK_H_ */