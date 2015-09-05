void ReadLidar() 
{
  if (IS_OK(lidar.waitPoint())) {
    ScanData(MONITOR_MODE);
  } else {
    DetectRPLidar(); // detect scaner
  }  
}

void ScanData(ScanMode mode)
{
  float distance = lidar.getCurrentPoint().distance; //distance value in mm unit
  float angle    = lidar.getCurrentPoint().angle; //anglue value in degree
  bool  startBit = lidar.getCurrentPoint().startBit; //whether this point is belong to a new scan
  byte  quality  = lidar.getCurrentPoint().quality; //quality of the current measurement
  
  distanceCM = int(distance/10); // 0 - 600 cm
  
  switch(mode){
  case DEBUG_MODE:
    Serial.print(distance);
    Serial.print(" mm, ");
    Serial.print(distanceCM);
    Serial.print(" cm, ");
    Serial.println(angle);
    delay(100);
  break; 
  case MONITOR_MODE:
      PlotArray(distanceCM, angle);
//    SendByte(distanceCM, angle, startBit);
  break;
  case DEMO_MODE:
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
  break;
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

void PlotArray(unsigned int dist, float deg){
  if(deg<=maxDeg) distBuff[int(deg)] = dist;
  
  byte subBuff[subMax];
  for(int i=0;i<sizeof(subBuff);i++){
    subBuff[i] = distBuff[i+count*subMax];
  }
  
  SendArray(subBuff,count);
  count++;
  if(count>int(maxDeg/subMax)-1) count = 0;
}

void SendArray(byte array[], unsigned int cnt){
 byte checksum = 0;
 checksum = (array[0]+array[1]+array[2]+array[3]+byte(cnt))%255;
 
 // data[4]|count[1]|checksum[1], total 6bytes
// if(debug.available()>0) {
////    for(int i=0; i<sizeof(array); i++){
////      debug.write(array[i]);
////  //      checksum=checksum+array[i];
////    }
//  //    checksum=checksum%255;
//    debug.write(array, sizeof(array));
//    debug.write(byte(cnt));
//    debug.write(checksum);
//    debug.read();
//  }

 if(debug.available()>0) {
    debug.write(array[0]);
    debug.write(array[1]);
    debug.write(array[2]);
    debug.write(array[3]);
    debug.write(byte(cnt));
    debug.write(checksum);
    debug.read();
  }
  delay(1);
}

//void SendArray(byte array[], unsigned int cnt){
// byte checksum = 0;
// checksum = (array[0]+array[1]+array[2]+array[3]+array[4]
// +array[5]+array[6]+array[7]+array[8]+array[9])%255;
// 
// // data[10]|count[1]|checksum[1], total 12bytes
// if(debug.available()>0) {
////    for(int i=0; i<sizeof(array); i++){
////      debug.write(array[i]);
////  //      checksum=checksum+array[i];
////    }
//  //    checksum=checksum%255;
//    debug.write(array, sizeof(array));
//    debug.write(byte(cnt));
//    debug.write(checksum);
//    debug.read();
//  }
//  delay(1);
//}

void SendByte(unsigned int data1, float data2, bool data3)
{
  byte checksum = 0;
  byte buff[5];
//  memset(buff,0,5);
  
  buff[0] = lowByte(data1); // data1&255
  buff[1] = highByte(data1); // data1>>8
  buff[2] = lowByte(int(data2));
  buff[3] = highByte(int(data2));
  buff[4] = byte(data3);
  
  checksum = (buff[0]+buff[1]+buff[2]+buff[3]+buff[4])%255;
 
 if(debug.available()>0) {
    for(int i=0; i<sizeof(buff); i++){
      debug.write(buff[i]);
//      checksum=checksum+buff[i];
    }
    
//    checksum=checksum%255;
    debug.write(checksum);
    debug.read();
  }
  delay(1);
}
