const int threshold =500; // minimum distance

void ReadLidar() 
{
  if (IS_OK(lidar.waitPoint())) {
    ScanDist();
//    ScanDemo(); // get scaner values
  } else {
    DetectRPLidar(); // detect scaner
  }  
}

void ScanDist()
{
    float distance = lidar.getCurrentPoint().distance;
    float angle = lidar.getCurrentPoint().angle; 
    
//    debug.print(distance);
//    debug.print(", ");
//    debug.println(angle);
    
//    if (distance<threshold) { // under 500cm
//      direct = 4;
//      if (angle>=270 && angle<310 ) {
//        direct = 1; // detect left range and turn right
//      } else if (angle>50 && angle<=90) {
//        direct = 2; // detect right range and turn left
//      } else if (angle>=310 && angle<=360) {
////        direct = 3; // detect front range and go backward
//        direct = 1;
//      } else if (angle>=0 && angle<=50) {
////        obstacle = 3; // detect front range and go backward
//        direct = 2;
//      }

      // detected range(-60 deg. ~ 60 deg.)
      if (distance<threshold) { // under 500cm
        direct = 4;
        if (angle>=300 && angle<=360 ) {
          direct = 1; // detect left range and turn right
        } else if (angle>=0 && angle<=60) {
          direct = 2; // detect right range and turn left
        } else {
          direct = 0;
        } 
    } else {
      direct = 0;  
    }
}

void DetectRPLidar()
{
  analogWrite(RPLIDAR_MTR, 0); //stop the rplidar motor
  // try to detect RPLIDAR... 
  rplidar_response_device_info_t info;
  if (IS_OK(lidar.getDeviceInfo(info, 100))) {
     // detected...
     lidar.startScan();
     
     analogWrite(RPLIDAR_MTR, 255); // start motor rotating at max allowed speed
     delay(1000);
  }
}

void ScanDemo()
{
  float distance = lidar.getCurrentPoint().distance; //distance value in mm unit
  float angle    = lidar.getCurrentPoint().angle; //anglue value in degree
  bool  startBit = lidar.getCurrentPoint().startBit; //whether this point is belong to a new scan
  byte  quality  = lidar.getCurrentPoint().quality; //quality of the current measurement
  
  Serial.print(distance);
  Serial.print(" mm, ");
  Serial.print(int(distance/10.0));
  Serial.print(" cm, ");
  Serial.println(angle);
  delay(100);
}
