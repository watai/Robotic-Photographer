const int threshold = 500;

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
    
    if (distance<threshold) { // under 500cm
      if (angle>=210 && angle<310 ) {
        direct = 1; // detect left range and turn right
      } else if (angle>50 && angle<=150) {
        direct = 2; // detect right range and turn left
      } else if (angle>=310 && angle<=360) {
//        direct = 3; // detect front range and go backward
        direct = 1;
      } else if (angle>=0 && angle<=50) {
//        obstacle = 3; // detect front range and go backward
        direct = 2;
      }
    } else {
      direct = 0;  
    }
}

void DetectRPLidar()
{
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

void ScanDemo()
{
  float distance = lidar.getCurrentPoint().distance; //distance value in mm unit
  float angle    = lidar.getCurrentPoint().angle; //anglue value in degree
  bool  startBit = lidar.getCurrentPoint().startBit; //whether this point is belong to a new scan
  byte  quality  = lidar.getCurrentPoint().quality; //quality of the current measurement
  
  debug.print(distance);
  debug.print(" mm, ");
  debug.print(int(distance/10.0));
  debug.print(" cm, ");
  debug.println(angle);
  delay(100);
}
