# Motor Speed Control

This program is designed to control the speed of a motor using an optical sensor and a PID controller. The motor speed is measured in revolutions per minute (RPM) and the goal is to maintain the RPM at a desired level (150 RPM by default)

## Hardware setup

- The optical sensor is connected to pin 2 of the microcontroller
- The motor is connected to the L293d motor driver circuit with the enable pin connected to pin 9 and the motor control pin connected to pin 11
- A serial connection is established with the microcontroller to display the RPM (Rotation Per Minute) and other parameters on the serial monitor

## How it works

1. The motor is started by applying a command to the enable pin and setting the motor control pin to high
2. The optical sensor is used to measure the number of revolutions the motor makes in a one-second interval
3. The RPM is calculated by dividing the number of revolutions by the number of holes in the wheel (20 by default) and multiplying by 60
4. The PID algorithm is used to calculate the correction value to be applied to the duty cycle of the enable pin in order to maintain the RPM at the desired level
5. A saturation/limiter function is applied to the correction value to prevent large changes in the duty cycle
6. The program also calculates other parameters of the system such as overshoot, rising time, and settling time.

## Usage

1. Connect the hardware as described above
2. Upload the code to the microcontroller
3. Open the serial monitor to view the RPM and other parameters
4. Adjust the constants of the PID controller (Kp, Ki, and Kd) to fine-tune the performance of the system
5. Adjust the design_rpm variable to set the desired RPM level

## Dependencies

This program is written in C++ and requires an Arduino IDE to be uploaded to a compatible microcontroller. The `millis()` function from the Arduino library is used to measure time intervals.
