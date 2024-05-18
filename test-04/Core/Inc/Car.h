/*
 * Car.h
 *
 *  Created on: Apr 26, 2024
 *      Author: Max Gardenswartz
 */

#ifndef SRC_CAR_H_
#define SRC_CAR_H_

#include <cstring> // for strcopy
#include <cstdint> // for uint_32t
#include "stm32l4xx_hal.h"

class Car {
public:
	// Allocation
	char brand[50];
	char model[50];
	int year;
	TIM_HandleTypeDef* timer;


	// Methods
	Car(const char* w, const char* x, int y, uint64_t z, TIM_HandleTypeDef* timer);
	virtual ~Car();
	void drive();
private:
	uint32_t serial_number;
};

#endif /* SRC_CAR_H_ */
