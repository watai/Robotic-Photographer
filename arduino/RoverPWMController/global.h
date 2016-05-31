// Pin Assign
// I2C Pin SDA﻿﻿,SCL﻿,3V3,GND
// RPLidar 0(RX),1(TX),3V3(D11),GND
// TRex SwSerial Pin 7(TX),8(RX)
// Xbee SwSerial Pin 3(TX), 2(RX)
// RC Pulse Input Pin 5,6
// Servo Pin 9,10

#define RC_CH1 5
#define RC_CH2 6
#define PWM_CH1 9
#define PWM_CH2 10
#define TXPIN 7
#define RXPIN 8
#define XBEE_TX 3
#define XBEE_RX 2
#define RPLIDAR_MTR 11

enum TrexMode{
  SERIAL_MODE,
  PWM_MODE
};

enum ScanMode{
  DEBUG_MODE,
  MONITOR_MODE,
  DEMO_MODE
};
