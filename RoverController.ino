#include <RPLidar.h>
#include <SoftwareSerial.h>
#include <MsTimer2.h>

#define TXPIN 4
#define RXPIN 5
SoftwareSerial trex(RXPIN, TXPIN);

#define DTXPIN 6
#define DRXPIN 7
SoftwareSerial debug(DRXPIN, DTXPIN);

RPLidar lidar;
#define RPLIDAR_MOTOR 3
unsigned int direct = 0;
bool bSteered = false;

void Motor() {
  if (!bSteered) {
    switch(direct){
    case 0: // go forward
        Forward(500);
//        ForwardAccel(500);
        debug.println("go forward");
    break;
    case 1: // turn right 
        bSteered = true;
        SteerRight(2000);
//        SteerRightAccel(1000);
        debug.println("turn right");
        bSteered = false;
    break;
    case 2: // turn left
        bSteered = true;
        SteerLeft(2000);
//        SteerLeftAccel(1000);
        debug.println("turn left");
        bSteered = false;
    break;
    case 3: // backward
        Backward(500);
//        BackwardAccel(500);
        debug.println("go backward");
    case 4: // stop
        SetStop(500);
        debug.println("Stop");
    break;
    default:
        SetStop(1000);
    break;
    }
  }
}

void setup() {
  pinMode(RXPIN, INPUT);
  pinMode(TXPIN, OUTPUT);
  pinMode(DRXPIN, INPUT);
  pinMode(DTXPIN, OUTPUT);
  pinMode(RPLIDAR_MOTOR, OUTPUT);
  
  lidar.begin(Serial);
  trex.begin(19200);
  debug.begin(9600);
  delay(5);

  MsTimer2::set(500, Motor); // 500ms
  MsTimer2::start();
}

void loop() {
  ReadLidar();
}

