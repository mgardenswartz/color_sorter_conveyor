/*
 * Car.cpp
 *
 *  Created on: Apr 26, 2024
 *      Author: Max Gardenswartz
 */

#include "Car.h"

Car::Car(const char* w, const char* x, int y, uint64_t z, TIM_HandleTypeDef* timer):
	timer(timer) {
    std::strcpy(brand, w);
    std::strcpy(model, x);
    year = y;
    serial_number = z;
}

Car::~Car() {
	// TODO Auto-generated destructor stub
}

void Car::drive() {
}


