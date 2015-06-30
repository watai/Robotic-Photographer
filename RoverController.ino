#include <RPLidar.h>
#include <SoftwareSerial.h>

#define TXPIN 4
#define RXPIN 5
SoftwareSerial trex(RXPIN, TXPIN);

RPLidar lidar;
#define RPLIDAR_MOTOR 3

#define DTXPIN 6
#define DRXPIN 7
SoftwareSerial debug(DRXPIN, DTXPIN);

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
}

void loop() {
//  readMotorCmd();
  if (IS_OK(lidar.waitPoint())) {
    RPLidarDemo(); // get scaner values
  } else {
    detectRPLidar(); // detect scaner
  } 
  
//  for (int i=0;i<5;i++){
//    steer(i);
//    debug.print("state:");
//    debug.println(i);
//    delay(3000);
//   }
   
//  MotorDemo(); // trex control
}

#define SPEED_HALF 64
#define SPEED_QUART 32
void steer(int state){
// Moto1 is left side, Motor2 is right side
// front is battery side
  switch(state){
    case 0: // forward
      SetSpeed(0, true, SPEED_QUART); 
      SetSpeed(1, true, SPEED_QUART);
      delay(100);
    break;
    case 1: // backward
      SetSpeed(0, false, SPEED_QUART);
      SetSpeed(1, false, SPEED_QUART);
      delay(100);
    break;
    case 2: // rotate right
      SetSpeed(0, true, SPEED_QUART);
      SetSpeed(1, true, 0);
      delay(300);
    break;
    case 3: // rotate left
      SetSpeed(0, true, 0);
      SetSpeed(1, true, SPEED_QUART);
      delay(300);
    break;
    case 4: // stop
      SetSpeed(0, true, 0);
      SetSpeed(1, true, 0);
      delay(1000);
    break;
    }
 }
  
void MotorDemo(){
  // Loop through 127 to 0, forward
  for(int i = 127; i >= 0; i--)
  {
    // Say that we are setting our speed
    debug.print("Setting speed 0 to: ");
    debug.println(i, DEC);
    SetSpeed(0, false, i); // Set speed to motor 0 and forward
    delay(100);
    
    // Say that we are setting our speed
    debug.print("Setting speed 1 to: ");
    debug.println(i, DEC);
    SetSpeed(1, false, i); // Set speed to motor 1 and forward
    delay(10);
  }

  // Loop through 0 to 127, backward
  for(int i = 0; i < 128; i++)
  {
    // Say that we are setting our speed
    debug.print("Setting speed 0 to: ");
    debug.println(i, DEC);
    SetSpeed(0, true, i); // Set speed to motor 0 and forward
    delay(100);
    
    // Say that we are setting our speed
    debug.print("Setting speed 1 to: ");
    debug.println(i, DEC);
    SetSpeed(1, true, i); // Set speed to motor 1 and forward
    delay(100);
  }
}

//void readMotorCmd(){
//  digitalWrite(12,LOW);
//  
//  debug.listen();
//  if (debug.available()>0) {
//  char cmd = debug.read();
//  debug.print("echo...");
//  debug.println(cmd);
//  if(cmd=='a')digitalWrite(12,HIGH);
//  }
//}

void SetSpeed(int MotorIndex, boolean Forward, int Speed){
  // Validate motor index
  if(MotorIndex < 0 || MotorIndex > 2)
    return;

  // Validate speed
  if(Speed < 0)
    Speed = 0;
  else if(Speed > 127)
    Speed = 127;

  // Send the "set" command based on the motor
  // Note that we do not accelerate to the
  // speed, we just instantly set it
  unsigned char SendByte = 0;
  if(MotorIndex == 0)
    SendByte = 0xC2; // set motor1 0xC0-0xC3
  else if(MotorIndex == 1)
    SendByte = 0xCA; // set motor2 0xC8-0xCB
  else if(MotorIndex == 2)
    SendByte = 0xF0; // set aux motor

  // If we go backwards, the commands are the same
  // but minus one
  if(!Forward)
    SendByte--;

  // Send the set speed command byte
  trex.write(SendByte);
  // Send the speed data byte
  trex.write(Speed);
}

void SetAccel(int MotorIndex, boolean Forward, int Speed){
  // Validate motor index
  if(MotorIndex < 0 || MotorIndex > 2)
    return;

  // Validate speed
  if(Speed < 0)
    Speed = 0;
  else if(Speed > 127)
    Speed = 127;
    
  unsigned char SendByte = 0;
  if(MotorIndex == 0)
    SendByte = 0xC6; // set motor1 0xC4-0xC7
  else if(MotorIndex == 1)
    SendByte = 0xCE; // set motor2 0xCC-0xCF
  else if(MotorIndex == 2)
    SendByte = 0xF1; // set aux motor
  
  // If we go backwards, the commands are the same
  // but minus one
  if(!Forward)
    SendByte--;

  // Send the set speed command byte
  trex.write(SendByte);
  // Send the speed data byte
  trex.write(Speed);
}

float minDistance = 100000;
float angleAtMinDist = 0;

void readRPLidar(){
    float distance = lidar.getCurrentPoint().distance;
    float angle = lidar.getCurrentPoint().angle;
    
    if (lidar.getCurrentPoint().startBit) {
      // a new scan, display the previous data...
//       displayColor(angleAtMinDist, minDistance);
       minDistance = 50000; // 500cm
       angleAtMinDist = 0;
    } else {
       if ( distance > 0 &&  distance < minDistance) {
          minDistance = distance;
          angleAtMinDist = angle;
       }
    }
}


void RPLidarDemo(){
  float distance = lidar.getCurrentPoint().distance; //distance value in mm unit
  float angle    = lidar.getCurrentPoint().angle; //anglue value in degree
  bool  startBit = lidar.getCurrentPoint().startBit; //whether this point is belong to a new scan
  byte  quality  = lidar.getCurrentPoint().quality; //quality of the current measurement
  
  debug.print(distance);
  debug.print(" mm, ");
  debug.print(int(distance/10.0));
  debug.print(" cm, ");
  debug.println(angle);
  delay(500);
}

void detectRPLidar(){
  analogWrite(RPLIDAR_MOTOR, 0); //stop the rplidar motor
  // try to detect RPLIDAR... 
  rplidar_response_device_info_t info;
  if (IS_OK(lidar.getDeviceInfo(info, 100))) {
     // detected...
     lidar.startScan();
     
     analogWrite(RPLIDAR_MOTOR, 255); // start motor rotating at max allowed speed
     delay(1000);
  }
}

