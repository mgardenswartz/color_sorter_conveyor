This code runs on an STM32F411CEU6. 

It's a part of a conveyor belt that sorts blocks by their color.
It runs a time-of-flight sensor first to see if a block has passes through the system.
Then, the position of the block is tracked internally with an encoder on the motor running the conveyor belt.
The block keeps travelling, and the color sensor reads its colors. Two servos guide the block to one of two bins at the end of the conveyor.

Other code is present here, too, that leads up to the term project. Here's a table of contents. 

Name	Description
lab-00	Blink an LED without PWM and count the blinks over UART
lab-01	Blink an LED with PWM
lab-02	Run two Maxon motors with classes and UI
lab-03	BlackPIll RC Lab
test-00	Toggle the Green LED (A5) by polling the user Button (B1)
test-03	Toggle the Green LED (A5) by generating an interrupt from the user Button (B1)
test-06	UART Interrupts test
test-07	Run a Maxon motor
test-08	BlackPill UART tests
test-11	lab-03 Checkpoint
![image](https://github.com/mgardenswartz/color_sorter_conveyor/assets/134117510/faa48d93-8eeb-4bce-9cc4-0a0461816d12)
