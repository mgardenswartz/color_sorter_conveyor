/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <cmath> // for fabs
#include "defines.h"
#include "vl53l0x_api.h" // ToF Sensor API with one small edit for porting to C++.
#include "TB6612FNG_Motor.h"
#include "RCChannel.h"
#include "RemoteControlControl.h"
#include "PololuEncoder.h"
#include "MotorControl.h"
#include "PololuServo.h"
#include "SparkFunAPDS9960.h"
#include "ConveyorBelt.h"
#include "ColoredBlock.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CPU_CLOCK_SPEED_MHZ 100
#define MOTOR_PWM_HZ 20000 // Should be at least 20,000 for hearing protection
#define RC_SIGNAL_PERIOD_US 17500
#define CONTROL_FREQUENCY_HZ 8
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim5;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
char my_message[MESSAGE_LENGTH] = "";
int string_length = MESSAGE_LENGTH;
bool initialized = false;

// Pointers
TB6612FNG_Motor* My_Motor;
RCChannel* ThrottleChannel;
RCChannel* SteeringChannel;
RemoteControlControl* Throttle;
RemoteControlControl* Steering;
PololuEncoder* My_Encoder;
MotorControl* My_Controller;
PololuServo* My_Servo;
ConveyorBelt* My_Conveyor;
SparkFun_APDS9960* My_Color_Sensor;
ColoredBlock* My_Block;

// C Data for ToF Sensor
VL53L0X_RangingMeasurementData_t RangingData;
VL53L0X_Dev_t vl53l0x_c; // Center module
VL53L0X_DEV Dev = &vl53l0x_c;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM5_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM3_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
static void VL53L0X_Init(void);
void I2C_Scan(UART_HandleTypeDef* uart_handle);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (initialized)
	{
		// The followning must be changed if the pin where the RC receiver is plugged in changes.
		if((htim->Instance = TIM1))
		{
			if ( htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1 )
			{
				SteeringChannel->callback();
			}

			if ( htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
			{
				ThrottleChannel->callback();
			}
		}
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USART2_UART_Init();
	MX_TIM2_Init();
	MX_TIM5_Init();
	MX_TIM1_Init();
	MX_TIM3_Init();
	MX_I2C1_Init();
	/* USER CODE BEGIN 2 */
	My_Motor = new TB6612FNG_Motor
		  (
		  GPIOB, GPIO_PIN_14,
		  GPIOB, GPIO_PIN_15,
		  &htim2, TIM_CHANNEL_1,
		  CPU_CLOCK_SPEED_MHZ,
		  MOTOR_PWM_HZ
		  );

	ThrottleChannel = new RCChannel
				  (
				  GPIOA,
				  GPIO_PIN_9,
				  &htim1,
				  TIM_CHANNEL_2,
				  RC_SIGNAL_PERIOD_US,
				  CPU_CLOCK_SPEED_MHZ,
				  1056,  // Calibrated full reverse
				  1533,  // Calibrated neutral
				  2017,  // Calibrated full speed
				  10000,   // New max positive
				  10000,    // New max negative (magnitude)
				  true  // Saturate if signal is over full? True by default. False good for debugging.
				  );

	Throttle = new RemoteControlControl
		  (
		  ThrottleChannel,
		  My_Controller,
		  30
		  );

	SteeringChannel = new RCChannel
		  (
		  GPIOA,
		  GPIO_PIN_8,
		  &htim1,
		  TIM_CHANNEL_1,
		  RC_SIGNAL_PERIOD_US,
		  CPU_CLOCK_SPEED_MHZ,
		  1002,       // Calibrated full right
		  1520,      // Calibrated neutral
		  2035,      // Calibrated full left
		  10000,       // New max positive
		  10000,       // New max negative (magnitude)
		  true     // Saturate if signal is over full? True by default. False good for debugging.
		  );

	My_Encoder = new PololuEncoder
		  (
		  &htim5,
		  20,
		  488.3f,
		  16,
		  false // True doesn't work yet!
		  );

	My_Controller = new MotorControl
		  (
		  My_Motor,
		  My_Encoder,
		  CONTROL_FREQUENCY_HZ,
		  100.0f,
		  650.0f,
		  0.0f
		  );

	My_Servo = new PololuServo
		  (
		  &htim3,
		  TIM_CHANNEL_1,
		  17500,
		  CPU_CLOCK_SPEED_MHZ,
		  false,  // Closed loop mode not implemented yet
		  60,
		  1000,
		  1500,
		  2000
		  );

	My_Conveyor = new ConveyorBelt
			(
			My_Encoder,
			38.1f // mm
			);

	// Color Sensor
	My_Color_Sensor = new SparkFun_APDS9960
			(
			&hi2c1
			);
	My_Color_Sensor->init();
	My_Color_Sensor->disableProximitySensor();
	My_Color_Sensor->disableGestureSensor();
	My_Color_Sensor->enableLightSensor();
	uint16_t ambient_light, red_light, green_light, blue_light;

	// ToF_Sensor in C (Not C++)
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
	VL53L0X_Init();

	// FSM Overview
	uint8_t task = 0;
	int number_of_states = 5;

	// Don't touch.
	number_of_states += 1;
	uint32_t last_tick_state[number_of_states] = {0};
	uint16_t task_frequencies[number_of_states] = {0};
	uint16_t range_reading = -1;
	bool ready_for_new_block = true;

	// Change the task frequencies here.
	task_frequencies[1] = CONTROL_FREQUENCY_HZ; // Hz
	task_frequencies[2] = 2; // Hz
	task_frequencies[3] = 10; // Hz
	task_frequencies[4] = 2; // Hz

	// Don't touch. Loop to calculate periods based on frequencies
	uint16_t task_periods[number_of_states] = {0};
	for (int index = 0; index < number_of_states; index++)
	{
		if (task_frequencies[index] != 0)
		{
		  task_periods[index] = 1000 / task_frequencies[index];
		}
		else
		{
		  task_periods[index] = 0; // Avoid division by zero
		}
	}

	// Other preferences
	uint16_t block_range_mm = 50; //Threshold
	float distance_from_ToF_to_color_sensor = 42.0f;
	int8_t conveyor_speed = -15;

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
	/* USER CODE END WHILE */

	/* USER CODE BEGIN 3 */
	  uint32_t current_tick = HAL_GetTick();

	  switch(task)
	  {
		  case 0:
			  // Init
			  My_Servo->set_position(0);
			  My_Controller->run(0);
			  HAL_Delay(500);
			  initialized = true;

			  // State change
			  task = 1;
			  break;

		  case 1: // RC and Motor Controller
			  int next_task;

			  if (current_tick - last_tick_state[1] >= task_periods[1])
			  {
				  last_tick_state[1] = current_tick;

				  if (-ThrottleChannel->value >= 0.6*ThrottleChannel->rescale_forward_magnitude)
				  {
					 My_Controller->run(conveyor_speed);
					 next_task = 2;
				  }
				  else
				  {
					My_Controller->run(0);
					next_task = 1;
				  }
			  }

			  // State change
			  task = next_task;
			  break;


		  case 2: // ToF Sensor
			  if (current_tick - last_tick_state[2] >= task_periods[2])
			  {
				  last_tick_state[2] = current_tick;

				  // Read ToF Sensor
				  VL53L0X_PerformSingleRangingMeasurement(Dev, &RangingData);

				if(ready_for_new_block)
				{

					if(RangingData.RangeMilliMeter <= block_range_mm)
					{
						// Debug message
						int string_length = sprintf(my_message, "A block was found\r\n");
						HAL_UART_Transmit(&huart2, (uint8_t *)my_message, string_length, HAL_MAX_DELAY);

						// Store the current encoder posiiton for later.
						My_Block->set_home();
						string_length = sprintf(my_message, "Block position: %.3f mm.\r\n", My_Block->home);
						HAL_UART_Transmit(&huart2, (uint8_t *)my_message, string_length, HAL_MAX_DELAY);

						// Stop checking for blocks.
						ready_for_new_block = false;

						// Create a new block instance
						My_Block = new ColoredBlock(My_Conveyor);
					}
				}
			  }

			  // State Changes
			  task = 4;
			  break;

		  case 4: // Color Sensor
			  if (current_tick - last_tick_state[4] >= task_periods[4])
			  {
				  last_tick_state[4] = current_tick;

				  if (not ready_for_new_block)
				  {
					  // Debug message
					  My_Block->update_position();
					  int string_length = sprintf(my_message, "Block position: %.2f mm.\r\n", fabs(My_Block->update_position()));
					  HAL_UART_Transmit(&huart2, (uint8_t *)my_message, string_length, HAL_MAX_DELAY);

					  if (fabs(My_Block->update_position()) >= distance_from_ToF_to_color_sensor)
					  {
						   // Debug message
							string_length = sprintf(my_message, "Block has reached color sensor \r\n");
							HAL_UART_Transmit(&huart2, (uint8_t *)my_message, string_length, HAL_MAX_DELAY);

						  // Update position of block
						  My_Block->update_position();

						  // Read the color sensor
	//						   My_Color_Sensor->readAmbientLight(ambient_light);
	//						   My_Color_Sensor->readRedLight(red_light);
	//						   My_Color_Sensor->readGreenLight(green_light);
	//						   My_Color_Sensor->readBlueLight(blue_light);

							bool success = My_Color_Sensor->readAmbientLight(ambient_light) &&
										   My_Color_Sensor->readRedLight(red_light) &&
										   My_Color_Sensor->readGreenLight(green_light) &&
										   My_Color_Sensor->readBlueLight(blue_light);


							if (success) {
								int string_length = sprintf(my_message, "Ambient: %u, Red: %u, Green: %u, Blue: %u\r\n", ambient_light, red_light, green_light, blue_light);
								HAL_UART_Transmit(&huart2, (uint8_t *)my_message, string_length, HAL_MAX_DELAY);
							} else {
								int string_length = sprintf(my_message, "Error reading light values!\r\n");
								HAL_UART_Transmit(&huart2, (uint8_t *)my_message, string_length, HAL_MAX_DELAY);
							}

						  // Update the color of the block
						  My_Block->update_color(red_light >= 100);

						  // Read for new block
						  ready_for_new_block = true;

						  // Servo Position
						  if (red_light >= 100)
						  {
							  My_Servo->set_position(-35);
						  }
						  else
						  {
							  My_Servo->set_position(40);
						  }

						  // Delete object
						  delete My_Block;
					  }
				  }
			  }

			 // State changes
			 task = 5;
			 break;

		  case 5: // Debug messages
			  if (current_tick - last_tick_state[5] >= task_periods[5])
			  {
				  last_tick_state[5] = current_tick;
				  // My_Controller->debug_message(&huart2);
				  // Throttle->debug_message(&huart2);
	//					bool success = My_Color_Sensor->readAmbientLight(ambient_light) &&
	//								   My_Color_Sensor->readRedLight(red_light) &&
	//								   My_Color_Sensor->readGreenLight(green_light) &&
	//								   My_Color_Sensor->readBlueLight(blue_light);
	//
	//
	//					if (success) {
	//						int string_length = sprintf(my_message, "Ambient: %u, Red: %u, Green: %u, Blue: %u\r\n", ambient_light, red_light, green_light, blue_light);
	//						HAL_UART_Transmit(&huart2, (uint8_t *)my_message, string_length, HAL_MAX_DELAY);
	//					} else {
	//						int string_length = sprintf(my_message, "Error reading light values!\r\n");
	//						HAL_UART_Transmit(&huart2, (uint8_t *)my_message, string_length, HAL_MAX_DELAY);
	//					}
					//	  			  int string_length = sprintf(my_message, "Channel value: %i/%i.\r\n",ThrottleChannel->value,ThrottleChannel->rescale_forward_magnitude);
					//	  			  HAL_UART_Transmit(&huart2, (uint8_t *)my_message, string_length, HAL_MAX_DELAY);


				  // Is a block within range?
				  if (range_reading < block_range_mm)
				  {
					  sprintf(my_message, "A block is within range\r\n");
					  HAL_UART_Transmit(&huart2, (uint8_t*)my_message, MESSAGE_LENGTH, HAL_MAX_DELAY);
				  }
			  }

			  // State change
			  task = 1;
			  break;

		  default:
			  task = 0;
			  break;
	  }
	}
	/* USER CODE END 3 */
	}

	/**
	* @brief System Clock Configuration
	* @retval None
	*/
	void SystemClock_Config(void)
	{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	*/
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	* in the RCC_OscInitTypeDef structure.
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 100;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
	Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
							  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
	{
	Error_Handler();
	}
	}

	/**
	* @brief I2C1 Initialization Function
	* @param None
	* @retval None
	*/
	static void MX_I2C1_Init(void)
	{

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 100000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	{
	Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

	}

	/**
	* @brief TIM1 Initialization Function
	* @param None
	* @retval None
	*/
	static void MX_TIM1_Init(void)
	{

	/* USER CODE BEGIN TIM1_Init 0 */

	/* USER CODE END TIM1_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_IC_InitTypeDef sConfigIC = {0};

	/* USER CODE BEGIN TIM1_Init 1 */

	/* USER CODE END TIM1_Init 1 */
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 100-1;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 65535;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
	{
	Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
	{
	Error_Handler();
	}
	if (HAL_TIM_IC_Init(&htim1) != HAL_OK)
	{
	Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
	{
	Error_Handler();
	}
	sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 0;
	if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
	{
	Error_Handler();
	}
	if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
	{
	Error_Handler();
	}
	/* USER CODE BEGIN TIM1_Init 2 */

	/* USER CODE END TIM1_Init 2 */

	}

	/**
	* @brief TIM2 Initialization Function
	* @param None
	* @retval None
	*/
	static void MX_TIM2_Init(void)
	{

	/* USER CODE BEGIN TIM2_Init 0 */

	/* USER CODE END TIM2_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};

	/* USER CODE BEGIN TIM2_Init 1 */

	/* USER CODE END TIM2_Init 1 */
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 0;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 5000-1;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
	{
	Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
	{
	Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
	{
	Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
	{
	Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
	Error_Handler();
	}
	/* USER CODE BEGIN TIM2_Init 2 */

	/* USER CODE END TIM2_Init 2 */
	HAL_TIM_MspPostInit(&htim2);

	}

	/**
	* @brief TIM3 Initialization Function
	* @param None
	* @retval None
	*/
	static void MX_TIM3_Init(void)
	{

	/* USER CODE BEGIN TIM3_Init 0 */

	/* USER CODE END TIM3_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};

	/* USER CODE BEGIN TIM3_Init 1 */

	/* USER CODE END TIM3_Init 1 */
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 0;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 5000-1;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
	{
	Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
	{
	Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
	{
	Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
	{
	Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
	Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
	{
	Error_Handler();
	}
	/* USER CODE BEGIN TIM3_Init 2 */

	/* USER CODE END TIM3_Init 2 */
	HAL_TIM_MspPostInit(&htim3);

	}

	/**
	* @brief TIM5 Initialization Function
	* @param None
	* @retval None
	*/
	static void MX_TIM5_Init(void)
	{

	/* USER CODE BEGIN TIM5_Init 0 */

	/* USER CODE END TIM5_Init 0 */

	TIM_Encoder_InitTypeDef sConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	/* USER CODE BEGIN TIM5_Init 1 */

	/* USER CODE END TIM5_Init 1 */
	htim5.Instance = TIM5;
	htim5.Init.Prescaler = 0;
	htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim5.Init.Period = 4294967295;
	htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
	sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
	sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
	sConfig.IC1Filter = 0;
	sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
	sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
	sConfig.IC2Filter = 0;
	if (HAL_TIM_Encoder_Init(&htim5, &sConfig) != HAL_OK)
	{
	Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
	{
	Error_Handler();
	}
	/* USER CODE BEGIN TIM5_Init 2 */

	/* USER CODE END TIM5_Init 2 */

	}

	/**
	* @brief USART2 Initialization Function
	* @param None
	* @retval None
	*/
	static void MX_USART2_UART_Init(void)
	{

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
	Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

	}

	/**
	* @brief GPIO Initialization Function
	* @param None
	* @retval None
	*/
	static void MX_GPIO_Init(void)
	{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

	/*Configure GPIO pins : PB10 PB12 PB14 PB15 */
	GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : PB13 */
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void I2C_Scan(UART_HandleTypeDef* uart_handle) {

	// Start
    sprintf(my_message, "Scanning...\r\n");
    HAL_UART_Transmit(uart_handle, (uint8_t*)my_message, strlen(my_message), HAL_MAX_DELAY);

    // Find
    for (uint8_t i = 1; i < 128; i++)
    {
        HAL_StatusTypeDef result = HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(i << 1), 1, 10);
        if (result == HAL_OK) {
            snprintf(my_message, sizeof(my_message), "Device found at address 0x%02X\r\n", i);
            HAL_UART_Transmit(&huart2, (uint8_t*)my_message, strlen(my_message), HAL_MAX_DELAY);
        }
    }

    // Finsish
    sprintf(my_message, "Done.\r\n\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t*) my_message, strlen(my_message), HAL_MAX_DELAY);
    HAL_Delay(1000); // Unnecessary
}

// VL53L0X time-of-flight sensor initialization
static void VL53L0X_Init(void) {
	uint32_t refSpadCount;
	uint8_t isApertureSpads;
	uint8_t VhvSettings;
	uint8_t PhaseCal;

	Dev->I2cHandle = &hi2c1;
	Dev->I2cDevAddr = 0x29 << 1;

	VL53L0X_WaitDeviceBooted(Dev);
	VL53L0X_DataInit(Dev);
	VL53L0X_StaticInit(Dev);
	VL53L0X_PerformRefCalibration(Dev, &VhvSettings, &PhaseCal);
	VL53L0X_PerformRefSpadManagement(Dev, &refSpadCount, &isApertureSpads);
	VL53L0X_SetDeviceMode(Dev, VL53L0X_DEVICEMODE_SINGLE_RANGING);

	VL53L0X_SetLimitCheckEnable(Dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
	VL53L0X_SetLimitCheckEnable(Dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);
	VL53L0X_SetLimitCheckValue(Dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, (FixPoint1616_t) (0.1*65536));
	VL53L0X_SetLimitCheckValue(Dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, (FixPoint1616_t) (60*65536));
	VL53L0X_SetMeasurementTimingBudgetMicroSeconds(Dev, 33000);
	VL53L0X_SetVcselPulsePeriod(Dev, VL53L0X_VCSEL_PERIOD_PRE_RANGE, 18);
	VL53L0X_SetVcselPulsePeriod(Dev, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, 14);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */