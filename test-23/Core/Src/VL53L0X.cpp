/**
 * @file VL53L0X.cpp
 * @brief Pololu Time of Flight sensor driver.
 * @author Max Gardenswartz
 * @date Jun 6, 2024
 */

#include "VL53L0X.h"

VL53L0X::VL53L0X(I2C_HandleTypeDef* I2C_handle,
				 uint8_t I2C_device_address
				 )
			     : I2C_handle(I2C_handle),
				   I2C_device_address(I2C_device_address)
{
	initialize();
}

uint16_t VL53L0X::read(UART_HandleTypeDef* uart_handle, bool debug)
{
	VL53L0X_PerformSingleRangingMeasurement(Dev, &RangingData);

	char my_message[MESSAGE_LENGTH] = "";
	int string_length;
	if(RangingData.RangeStatus == 0)
	{
		if(debug)
		{
			string_length = sprintf(my_message, "Measured distance: %i\r\n", RangingData.RangeMilliMeter);
			HAL_UART_Transmit(uart_handle, (uint8_t*)my_message, string_length, HAL_MAX_DELAY);
		}
		return RangingData.RangeMilliMeter;
	}
	else
	{
		if(debug)
		{
			string_length = sprintf(my_message, "Error. \r\n");
			HAL_UART_Transmit(uart_handle, (uint8_t*)my_message, string_length, HAL_MAX_DELAY);
		}
		return -1;
	}
}

void VL53L0X::initialize()
{
	Dev->I2cHandle = I2C_handle;
	Dev->I2cDevAddr = I2C_device_address << 1;

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

VL53L0X::~VL53L0X()
{
	// TODO Auto-generated destructor stub
}

