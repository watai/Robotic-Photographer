// library -> https://github.com/felis/USB_Host_Shield_2.0

#include <PS3BT.h>
#include <usbhub.h>
#include <Servo.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

#define PWM_CH1 5
#define PWM_CH2 6
Servo MTR_R;
Servo MTR_L;
// RC pulse
uint16_t pulse1, pulse2;
const unsigned int miniPulse = 1050;
const unsigned int maxPulse = 1950;
const unsigned int midPulse = 1500;

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
/* You can create the instance of the class in two ways */
PS3BT PS3(&Btd); // This will just create the instance
// This will also store the bluetooth address - this can be obtained from the dongle when running the sketch
//PS3BT PS3(&Btd, 0x00, 0x15, 0x83, 0x3D, 0x0A, 0x57);

bool printTemperature;
bool printAngle;
bool startFlag;

void setup() {
  Serial.begin(115200);
  MTR_L.attach(PWM_CH1, miniPulse, maxPulse);
  MTR_R.attach(PWM_CH2, miniPulse, maxPulse);
  MTR_L.writeMicroseconds(midPulse);
  MTR_R.writeMicroseconds(midPulse);

  // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#if !defined(__MIPSEL__)
  while (!Serial);
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));

  startFlag = false;
}

uint16_t remapRVal(uint8_t val) {
  uint16_t v;
  v = map(val, 255, 0, miniPulse, midPulse);
  if (v < miniPulse ) v = midPulse;
  return v;
}

uint16_t remapLVal(uint8_t val) {
  uint16_t v;
  v = map(val, 0, 255, midPulse, maxPulse);
  if (v > maxPulse) v = midPulse;
  return v;
}

uint16_t remapVal(uint8_t val) {
  uint16_t v;
  v = map(val, 255, 0, miniPulse, maxPulse);
  if (v < miniPulse || v > maxPulse) v = midPulse;
  return v;
}

bool ErrMsg(uint8_t x, uint8_t y) {
  if (x == 0 && y == 0) return false;
  else return true;
}

void loop() {
  Usb.Task();

  if (PS3.PS3Connected) {
    uint8_t leftHatX = PS3.getAnalogHat(LeftHatX);
    uint8_t leftHatY = PS3.getAnalogHat(LeftHatY);
    uint8_t rightHatX = PS3.getAnalogHat(RightHatX);
    uint8_t rightHatY = PS3.getAnalogHat(RightHatY);
    uint8_t L2Btn = PS3.getAnalogButton(L2);
    uint8_t R2Btn = PS3.getAnalogButton(R2);

    // Push button values
    if (PS3.getButtonClick(PS)) {
      PS3.disconnect();
    } else {
      if (PS3.getButtonClick(SELECT)) {
        Serial.print(F("\r\nSelect - "));
        PS3.printStatusString();
      }
      if (PS3.getButtonClick(START)) {
        startFlag = !startFlag;
        if (startFlag) {
          Serial.print(F("\r\nStart"));
        } else {
          Serial.print(F("\r\nStop"));
        }
      }
    }

    // Start to send pulse
    if (startFlag) {
      // control forward/back channel
      if (L2Btn || R2Btn) {
        if (L2Btn) {
          pulse2 = remapLVal(L2Btn); // go back
          Serial.print(F("\r\nL2: "));
          Serial.print(L2Btn);
        } else {
          pulse2 = remapRVal(R2Btn); // go forward
          Serial.print(F("\r\nR2: "));
          Serial.print(R2Btn);
        }
        MTR_R.writeMicroseconds(pulse2);
        Serial.print(F("\tPulse2: "));
        Serial.print(pulse2);
      } else {
        // set center positon
        pulse2 = midPulse;
        MTR_R.writeMicroseconds(pulse2);
      }

      // control stering channel
      if (rightHatX > 171 || rightHatX < 85) {
        pulse1 = remapVal(rightHatX);
        MTR_L.writeMicroseconds(pulse1); // stering
      } else {
        // set center position
        pulse1 = midPulse;
        MTR_L.writeMicroseconds(pulse1);
      }
      Serial.print(F("\r\nPulse1: "));
      Serial.print(pulse1);
      Serial.print(F("\tRightHatX: "));
      Serial.print(rightHatX);

    } else {
      // set center position
      pulse1 = midPulse;
      pulse2 = midPulse;
      MTR_L.writeMicroseconds(pulse1);
      MTR_R.writeMicroseconds(pulse2);
    }

    // remapping analog hat value
    //    if (ErrMsg(leftHatX, leftHatY)) {
    //      if (leftHatX > 137 || leftHatX < 117) {
    //        pulse1 = remapVal(leftHatX);
    //        // send pulse to drive motor
    //        MTR_R.writeMicroseconds(pulse1);
    //
    //        Serial.print(F("\r\nPulse1: "));
    //        Serial.print(pulse1);
    //        Serial.print(F("\tLeftHatY: "));
    //        Serial.print(leftHatY);
    //        Serial.print(F("\tLeftHatX: "));
    //        Serial.print(leftHatX);
    //      }
    //    }

    //    if (ErrMsg(rightHatX, rightHatY)) {
    //      if (rightHatY > 137 || rightHatY < 117) {
    //        pulse2 = remapVal(rightHatY);
    //        // send pulse to drive motor
    //        MTR_L.writeMicroseconds(pulse2);
    //
    //        Serial.print(F("\r\nPulse2: "));
    //        Serial.print(pulse2);
    //        Serial.print(F("\tRightHatY: "));
    //        Serial.print(rightHatY);
    //        Serial.print(F("\tRightHatX: "));
    //        Serial.print(rightHatX);
    //      }
    //    }
  }
  //  reportMessage(true);
}

//void reportMessage(bool flag) {
//  if (flag) {
//    if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
//      if (PS3.getAnalogHat(LeftHatX) > 137 || PS3.getAnalogHat(LeftHatX) < 117
//          || PS3.getAnalogHat(LeftHatY) > 137 || PS3.getAnalogHat(LeftHatY) < 117
//          || PS3.getAnalogHat(RightHatX) > 137 || PS3.getAnalogHat(RightHatX) < 117
//          || PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117) {
//
//        Serial.print(F("\r\nLeftHatX: "));
//        Serial.print(PS3.getAnalogHat(LeftHatX));
//        Serial.print(F("\tLeftHatY: "));
//        Serial.print(PS3.getAnalogHat(LeftHatY));
//        if (PS3.PS3Connected) { // The Navigation controller only have one joystick
//          Serial.print(F("\tRightHatX: "));
//          Serial.print(PS3.getAnalogHat(RightHatX));
//          Serial.print(F("\tRightHatY: "));
//          Serial.print(PS3.getAnalogHat(RightHatY));
//        }
//      }
//
//      // Analog button values can be read from almost all buttons
//      if (PS3.getAnalogButton(L2) || PS3.getAnalogButton(R2)) {
//        Serial.print(F("\r\nL2: "));
//        Serial.print(PS3.getAnalogButton(L2));
//        if (PS3.PS3Connected) {
//          Serial.print(F("\tR2: "));
//          Serial.print(PS3.getAnalogButton(R2));
//        }
//      }
//      // Push button values
//      if (PS3.getButtonClick(PS)) {
//        Serial.print(F("\r\nPS"));
//        PS3.disconnect();
//      }
//      else {
//        if (PS3.getButtonClick(TRIANGLE))
//          Serial.print(F("\r\nTraingle"));
//        if (PS3.getButtonClick(CIRCLE))
//          Serial.print(F("\r\nCircle"));
//        if (PS3.getButtonClick(CROSS))
//          Serial.print(F("\r\nCross"));
//        if (PS3.getButtonClick(SQUARE))
//          Serial.print(F("\r\nSquare"));
//
//        if (PS3.getButtonClick(UP)) {
//          Serial.print(F("\r\nUp"));
//          if (PS3.PS3Connected) {
//            //            PS3.setLedOff();
//            //            PS3.setLedOn(LED4);
//          }
//        }
//        if (PS3.getButtonClick(RIGHT)) {
//          Serial.print(F("\r\nRight"));
//          if (PS3.PS3Connected) {
//            //            PS3.setLedOff();
//            //            PS3.setLedOn(LED1);
//          }
//        }
//        if (PS3.getButtonClick(DOWN)) {
//          Serial.print(F("\r\nDown"));
//          if (PS3.PS3Connected) {
//            //            PS3.setLedOff();
//            //            PS3.setLedOn(LED2);
//          }
//        }
//        if (PS3.getButtonClick(LEFT)) {
//          Serial.print(F("\r\nLeft"));
//          if (PS3.PS3Connected) {
//            //            PS3.setLedOff();
//            //            PS3.setLedOn(LED3);
//          }
//        }
//
//        if (PS3.getButtonClick(L1))
//          Serial.print(F("\r\nL1"));
//        if (PS3.getButtonClick(L3))
//          Serial.print(F("\r\nL3"));
//        if (PS3.getButtonClick(R1))
//          Serial.print(F("\r\nR1"));
//        if (PS3.getButtonClick(R3))
//          Serial.print(F("\r\nR3"));
//
//        if (PS3.getButtonClick(SELECT)) {
//          Serial.print(F("\r\nSelect - "));
//          PS3.printStatusString();
//        }
//        if (PS3.getButtonClick(START)) {
//          Serial.print(F("\r\nStart"));
//          printAngle = !printAngle;
//        }
//      }
//#if 0 // Set this to 1 in order to see the angle of the controller
//      if (printAngle) {
//        Serial.print(F("\r\nPitch: "));
//        Serial.print(PS3.getAngle(Pitch));
//        Serial.print(F("\tRoll: "));
//        Serial.print(PS3.getAngle(Roll));
//      }
//#endif
//    }
//  }
//}

