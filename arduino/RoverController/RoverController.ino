#include <RPLidar.h>
#include <SoftwareSerial.h>
#include <MsTimer2.h>
#define LEONARDO

#define TXPIN 4
#define RXPIN 5
SoftwareSerial trex(RXPIN, TXPIN);

#define DTXPIN 6
#define DRXPIN 7
SoftwareSerial debug(DRXPIN, DTXPIN);

RPLidar lidar;
#define RPLIDAR_MTR 3
unsigned int direct = 0;
bool bSteered = false;

void Motor() {
  if (!bSteered) {
    switch(direct){
    case 0: // go forward
        Forward(300);
//        ForwardAccel(500);
        debug.println("go forward");
    break;
    case 1: // turn right 
        bSteered = true;
        SteerRight(1600);
//        SteerRightAccel(1000);
        debug.println("turn right");
        bSteered = false;
    break;
    case 2: // turn left
        bSteered = true;
        SteerLeft(1600);
//        SteerLeftAccel(1000);
        debug.println("turn left");
        bSteered = false;
    break;
    case 3: // backward
        Backward(300);
//        BackwardAccel(500);
        debug.println("go backward");
    case 4: // stop
        SetStop(100);
        debug.println("Stop");
    break;
    default:
        SetStop(100);
    break;
    }
  }
}

void setup() {
  pinMode(RXPIN, INPUT);
  pinMode(TXPIN, OUTPUT);
  pinMode(DRXPIN, INPUT);
  pinMode(DTXPIN, OUTPUT);
  pinMode(RPLIDAR_MTR, OUTPUT);
  
#ifdef LEONARDO
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Start");
  lidar.begin(Serial1);
#else
  lidar.begin(Serial);
#endif

  trex.begin(19200);
  debug.begin(9600);
  delay(5);

  MsTimer2::set(300, Motor); // 500ms
  MsTimer2::start();
}

void loop() {
  ReadLidar();
}

