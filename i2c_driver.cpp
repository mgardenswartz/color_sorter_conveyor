#include <iostream>
#include <unordered_map>
#include <cstdint>
#include <string>
using namespace std;

uint8_t i2c_addr = 0x52;

// Create a dictionary of memory address for my I2C devices
unordered_map<string, uint8_t> i2c_memaddresses = {
    {"color_channel_red", 0x01},
    {"color_channel_orange", 0x02},
    {"color_channel_green", 0x03},
    {"color_channel_blue", 0x04},
    {"color_channel_violet", 0x05},
    {"color_channel_white", 0x06},
    {"mode", 0x07}
};

// Grab a value from the dictionary

int main()
{
    // Check if the I2C address is valid
    if ((i2c_addr < 0) || (i2c_addr > 0x7F))
    {
        cout << "Invalid I2C address" << endl;
        return 1;
    }

    // Access the dictionary values
    uint8_t device1_address = i2c_memaddresses["device1"];
    uint8_t device2_address = i2c_memaddresses["device2"];
    uint8_t device3_address = i2c_memaddresses["device3"];

    // Use the addresses in your code
    // Example:
    // HAL_I2C_Mem_Write(device1_address, ...)
    // HAL_I2C_Mem_Read(device2_address, ...)

    return 0;
}

