// RoverController via PWM
// BATT -> TRex Motor Driver - Arduino Leonardo - RC Reciever
// Pin Assign
// I2C Pin 2(SDA)﻿﻿,3(SCL)﻿,3V3,GND
// RPLidar 0(RX),1(TX),3V3(D11),GND
// TRex/Debug Serial Pin 7(TX),8(RX)
// RC Pulse Input Pin 5,6
// Servo Pin 9,10

#include <Servo.h>
#include <SoftwareSerial.h>
#include <RPLidar.h>

#define LEONARDO

#define RC_CH1 5
#define RC_CH2 6
#define PWM_CH1 9
#define PWM_CH2 10
#define TXPIN 7
#define RXPIN 8
#define RPLIDAR_MTR 11

Servo MTR_R;
Servo MTR_L;
int pulse_ch1;
int pulse_ch2;

//SoftwareSerial trex(RXPIN, TXPIN);

RPLidar lidar;
unsigned int direct = 0;
bool bSteered = false;

void setup() {
  pinMode(RC_CH1, INPUT);
  pinMode(RC_CH2, INPUT);
  pinMode(RXPIN, INPUT);
  pinMode(TXPIN, OUTPUT);
  pinMode(RPLIDAR_MTR, OUTPUT);
  
#ifdef LEONARDO
  Serial.begin(9600);
//  while (!Serial);
  lidar.begin(Serial1);
#else
  lidar.begin(Serial);
#endif

//  trex.begin(19200);
//  delay(5);

  MTR_R.attach(PWM_CH1,1000,2000);
  MTR_L.attach(PWM_CH2,1000,2000);
  MTR_R.writeMicroseconds(1500);
  MTR_L.writeMicroseconds(1500);
}

void loop() {
//  ReadLidar();
  ThruPulse();
}


