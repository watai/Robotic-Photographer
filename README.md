# Robotic Photographer
The Rover Unmanned Ground Vehicle for Photography.

![robo](/image/robo640.jpg)
<!-- <img src="/image/robo1.jpg" width="640px"> -->

## Purpose

## Features
- Autonomous Control
- Scanning Environment with Leaser Range Scaner
- Remote Control with PS3 or RC2.4GHz Controller
- Remote Shooting Photography
- Arduino-Compatible
- Unfortunately, this system hasn't been implemented autonomous shooting photo function. So, it's able to wireless remote control camera by hand.

## Hardware
Features:
- Power Supply: 7.4V2S 3300mAh LiPo Battery
- Arduino Uno Compatible
- USB Host Shield 2.0 for Arduino
- Chassis: [Dagu Wild Thumper 6WD All-Terrain Chassis, Black, 75:1](https://www.pololu.com/product/1563)

### Pololu TReX Dual Motor Controller
- Supply Voltage: 6 – 16 V
- Primary Motor Outputs: 2 bidirectional, 30 A peak, up to 13 A continuous per channel
- Auxiliary output: 1 unidirectional, 15 A continuous
- Two bidirectional and one unidirectional motor
- RC Pulse(PWM)/10bit Analog Voltage/RS232 or TTL Serial


### RPLIDAR 360° Laser Ranger System
- 360 degree 2D Laser Scanner
- 5-10Hz Adaptive Scan Frequency
- 2000 Times of Ranging per Second
- Range More than 6 Meters


### Components List
Please refer [here](docs/components.md).

## Firmware
### Dependencies
- [USB Host Library for Arduino](https://github.com/felis/USB_Host_Shield_2.0)
- [RPLIDAR driver for Arduino ](https://github.com/robopeak/rplidar_arduino)

## Reference
- [Pololu TReX Dual Motor Controller DMC01](https://www.pololu.com/product/777)
- [RPLIDAR A1](http://www.slamtec.com/en/Lidar/A1)
- [RPLIDAR ROS](https://github.com/robopeak/rplidar_ros)
