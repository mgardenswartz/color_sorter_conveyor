![image](https://github.com/mgardenswartz/color_sorter_conveyor/assets/134117510/98b3bc27-004b-46b7-9604-676cf0900080)

This code runs on an STM32F411CEU6. 

It's a part of a conveyor belt that sorts blocks by their color.
It runs a time-of-flight sensor first to see if a block has passes through the system.
Then, the position of the block is tracked internally with an encoder on the motor running the conveyor belt.
The block keeps travelling, and the color sensor reads its colors. Two servos guide the block to one of two bins at the end of the conveyor.

Other code is present here, too, that leads up to the term project. Here's a table of contents. 
