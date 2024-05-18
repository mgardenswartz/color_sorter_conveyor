This code runs on an STM32F411CEU6. 

It's a part of a conveyor belt that sorts blocks by their color.
It runs a time-of-flight sensor first to see if a block has passes through the system.
Then, the position of the block is tracked internally with an encoder on the motor running the conveyor belt.
The block keeps travelling, and the color sensor reads its colors. 
