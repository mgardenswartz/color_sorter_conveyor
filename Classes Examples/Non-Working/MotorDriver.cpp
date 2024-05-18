/*
 * MotorDriver.cpp
 *
 *  Created on: Apr 15, 2024
 *      Author: Max Gardenswartz
 */

// From stm32l4xx_hal_tim.h
//#define TIM_CHANNEL_1                      0x00000000U                          /*!< Capture/compare channel 1 identifier      */
//#define TIM_CHANNEL_2                      0x00000004U                          /*!< Capture/compare channel 2 identifier      */
//#define TIM_CHANNEL_3                      0x00000008U                          /*!< Capture/compare channel 3 identifier      */
//#define TIM_CHANNEL_4                      0x0000000CU                          /*!< Capture/compare channel 4 identifier      */
//#define TIM_CHANNEL_5                      0x00000010U                          /*!< Compare channel 5 identifier              */
//#define TIM_CHANNEL_6                      0x00000014U                          /*!< Compare channel 6 identifier              */
//#define TIM_CHANNEL_ALL                    0x0000003CU                          /*!< Global Capture/compare channel identifier  */

// From stm32l4xx_hal_gpio.h
//#define GPIO_PIN_0                 ((uint16_t)0x0001)  /* Pin 0 selected    */
//#define GPIO_PIN_1                 ((uint16_t)0x0002)  /* Pin 1 selected    */
//#define GPIO_PIN_2                 ((uint16_t)0x0004)  /* Pin 2 selected    */
//#define GPIO_PIN_3                 ((uint16_t)0x0008)  /* Pin 3 selected    */
//#define GPIO_PIN_4                 ((uint16_t)0x0010)  /* Pin 4 selected    */
//#define GPIO_PIN_5                 ((uint16_t)0x0020)  /* Pin 5 selected    */
//#define GPIO_PIN_6                 ((uint16_t)0x0040)  /* Pin 6 selected    */
//#define GPIO_PIN_7                 ((uint16_t)0x0080)  /* Pin 7 selected    */
//#define GPIO_PIN_8                 ((uint16_t)0x0100)  /* Pin 8 selected    */
//#define GPIO_PIN_9                 ((uint16_t)0x0200)  /* Pin 9 selected    */
//#define GPIO_PIN_10                ((uint16_t)0x0400)  /* Pin 10 selected   */
//#define GPIO_PIN_11                ((uint16_t)0x0800)  /* Pin 11 selected   */
//#define GPIO_PIN_12                ((uint16_t)0x1000)  /* Pin 12 selected   */
//#define GPIO_PIN_13                ((uint16_t)0x2000)  /* Pin 13 selected   */
//#define GPIO_PIN_14                ((uint16_t)0x4000)  /* Pin 14 selected   */
//#define GPIO_PIN_15                ((uint16_t)0x8000)  /* Pin 15 selected   */
//#define GPIO_PIN_All               ((uint16_t)0xFFFF)  /* All pins selected */


// Example Input
// PWM_TIMER_HANDLE = htim4
// PWM_CHANNEL = TIM_CHANNEL_1
// PIN1_GPIO_PORT = GPIOA
// PIN1 = GPIO_PIN_6
// PIN2_GPIO_PORT = GPIOA
// PIN2 = GPIO_PIN_8

#include "MotorDriver.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_gpio.h"
#include "stm32l4xx_hal_tim.h"

// Constructor
MotorDriver::MotorDriver(my_number)
		{
}

// Destructor
MotorDriver::~MotorDriver() {
    // Destructor
}



