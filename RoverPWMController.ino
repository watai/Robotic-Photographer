// RoverController via PWM
// RC Reciever -> Arduino UNO -> TRex Motor Driver

#include <Servo.h>
#include <SoftwareSerial.h>
#include <RPLidar.h>
#include "global.h"

Servo MTR_R;
Servo MTR_L;

//SoftwareSerial trex(RXPIN, TXPIN);
SoftwareSerial debug(RXPIN, TXPIN);
//SoftwareSerial debug(XBEE_RX, XBEE_TX);

RPLidar lidar;
unsigned int distanceCM = 0;
unsigned int count = 0;
const int maxDeg = 360;
const int subMax = 4;
byte distBuff[maxDeg];
const int threshold =500; // minimum distance

unsigned int direct = 0;
//bool bSteered = false;

void setup() {
  setMode(PWM_MODE);
  pinMode(RPLIDAR_MTR, OUTPUT);
  lidar.begin(Serial);
  
  memset(distBuff,0,sizeof(distBuff));
  
//  Serial.print(9600);
  debug.begin(9600); // sw serial monitor
}

void loop() {
//  PulseInOut();
  ReadLidar();
}

void setMode(TrexMode mode){
  switch(mode){
    case SERIAL_MODE:
      pinMode(RXPIN, INPUT);
      pinMode(TXPIN, OUTPUT);
//      trex.begin(19200);
    break;
    case PWM_MODE:
      pinMode(RC_CH1, INPUT);
      pinMode(RC_CH2, INPUT);
      MTR_R.attach(PWM_CH1,1000,2000);
      MTR_L.attach(PWM_CH2,1000,2000);
      MTR_R.writeMicroseconds(1500);
      MTR_L.writeMicroseconds(1500);
    break;
  }
}

void PulseInOut(){
  unsigned int pulse1, pulse2;
  pulse1 = pulseIn(RC_CH1, HIGH, 25000); // Read the pulse width of  
  pulse2 = pulseIn(RC_CH2, HIGH, 25000); // each channel
  
  if(pulse1<1000||pulse1>2000){pulse1=1500;}
  if(pulse2<1000||pulse2>2000){pulse2=1500;}
  
  MTR_R.writeMicroseconds(pulse1);
  MTR_L.writeMicroseconds(pulse2);
}



