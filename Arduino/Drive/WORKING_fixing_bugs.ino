

/*
Example sketch for the PS4 Bluetooth library - developed by Kristian Lauszus
For more information visit my blog: http://blog.tkjelectronics.dk/ or
send me an e-mail: kristianl@tkjelectronics.com
*/
// include the SPI library:
#include <SPI.h>

// L9958 DIRection pins
#define DIR_M1 2
#define DIR_M2 3
#define DIR_M3 4
#define DIR_M4 7

// L9958 PWM pins
#define PWM_M1 9     // Timer2
#define PWM_M2 10    // Timer2
#define PWM_M3 5     // Timer3
#define PWM_M4 6     // Timer4

// L9958 Enable for all 4 motors
#define ENABLE_MOTORS 8

int pwm1, pwm2, pwm3, pwm4;
int dir1, dir2, dir3, dir4;
int angle, tempangle;
float velocity;
int XHat, YHat;

const int M1[60] = {
0, 4, 9, 13, 18, 22, 27, 31, 35, 40, 
44, 49, 53, 57, 62, 66, 70,75, 79, 83,
87, 91, 96, 100, 104, 108, 112, 116, 120, 124, 
128, 131, 135, 139, 143, 146, 150, 153, 157, 160,
164, 167, 171, 174, 177, 180, 183, 186, 190, 192, 
195, 198, 201, 204, 206, 209, 211, 214, 216, 219
};
const int M2[60] = {
221, 219, 216, 214, 211, 209, 206, 204, 201, 198,
195, 192, 190, 186, 183, 180, 177, 174, 171, 167,
164, 160, 157, 153, 150, 146, 143, 139, 135, 131, 
128, 124, 120, 116, 112, 108, 104, 100, 96, 91,
87, 83, 79, 75, 70, 66, 62, 57, 53, 49,
44, 40, 35, 31, 27, 22, 18, 13, 9, 4
};
const int M3[60] = {
221, 223, 255, 227, 229, 231, 233, 235, 236, 238, 
240, 241, 243, 244, 245, 246, 247, 248, 249, 250, 
251, 252, 253, 253, 254, 254, 254, 255, 255, 255, 
255, 255, 255, 255, 254, 254, 254, 253, 253, 252, 
251, 250, 249, 248, 247, 246, 245, 244, 243, 241,
240, 238, 236, 235, 233, 231, 229, 277, 255, 223
};


#include <PS4BT.h>
#include <usbhub.h>



// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so

/* You can create the instance of the PS4BT class in two ways */
// This will start an inquiry and then pair with the PS4 controller - you only have to do this once
// You will need to hold down the PS and Share button at the same time, the PS4 controller will then start to blink rapidly indicating that it is in paring mode
//PS4BT PS4(&Btd, PAIR);

// After that you can simply create the instance like so and then press the PS button on the device
PS4BT PS4(&Btd);

boolean printAngle, printTouch;

void setup() {
  
  // L9958 DIRection pins
  pinMode(DIR_M1, OUTPUT);
  pinMode(DIR_M2, OUTPUT);
  pinMode(DIR_M3, OUTPUT);
  pinMode(DIR_M4, OUTPUT);
  
  // L9958 PWM pins
  pinMode(PWM_M1, OUTPUT);    digitalWrite(PWM_M1, LOW);    // Timer2 (Mega)
  pinMode(PWM_M2, OUTPUT);    digitalWrite(PWM_M2, LOW);    // Timer2 (Mega)
  pinMode(PWM_M3, OUTPUT);    digitalWrite(PWM_M3, LOW);    // Timer3 (Mega)
  pinMode(PWM_M4, OUTPUT);    digitalWrite(PWM_M4, LOW);    // Timer4 (Mega)
  
  // L9958 Enable for all 4 motors
  pinMode(ENABLE_MOTORS, OUTPUT);  digitalWrite(ENABLE_MOTORS, HIGH);   // HIGH = disabled
  
Serial.begin(115200);
while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
if (Usb.Init() == -1) {
Serial.print(F("\r\nOSC did not start"));
while (1); // Halt
}
Serial.print(F("\r\nPS4 Bluetooth Library Started"));
}
void loop() {
Usb.Task();

if (PS4.connected()) {
  
if (PS4.getAnalogHat(LeftHatX) > 137 || PS4.getAnalogHat(LeftHatX) < 117 || PS4.getAnalogHat(LeftHatY) > 138 || PS4.getAnalogHat(LeftHatY) < 118) {
//    || PS4.getAnalogHat(RightHatX) > 137 || PS4.getAnalogHat(RightHatX) < 117 || PS4.getAnalogHat(RightHatY) > 137 || PS4.getAnalogHat(RightHatY) < 117) {
//      Serial.print(F("\r\nLeftHatX: "));
//      Serial.print(modHatValue(PS4.getAnalogHat(LeftHatX)));
//      Serial.print(F("\tLeftHatY: "));
//      Serial.println(modHatValue(PS4.getAnalogHat(LeftHatY)));
XHat = modHatValue(PS4.getAnalogHat(LeftHatX));           //gets the modified LeftHatX Value
YHat = modHatValue(PS4.getAnalogHat(LeftHatY));           //gets the modified LeftHatY Value
velocity = calcVelocity(XHat, YHat);                      //calulates the magnitude of the velocity sent to the PWMs
//      Serial.print("Velocity is:\t");                       
//      Serial.println(velocity); 
tempangle = (atan2(XHat, -YHat)*4068)/71;                  //calculates the angle and converts rads to deg
angle = roundint(tempangle);                              // changes angle from float to int
if (angle < 0){
  angle = 360 + angle;                                    //makes degrees go from 0-360
  }
if (angle >= 0 && angle < 60){
  dir1 = 1; pwm1 = M1[angle];                            //Sets all three motors based on direction and velocity
  dir2 = 1; pwm2 = M2[angle];
  dir3 = 0; pwm3 = M3[angle];
  }
else if (angle >= 60 && angle < 120){
  dir1 = 1; pwm1 = M3[angle-60];
  dir2 = 0; pwm2 = M1[angle-60];
  dir3 = 0; pwm3 = M2[angle-60];
  }
else if (angle >= 120 && angle < 180){
  dir1 = 1; pwm1 = M2[angle-120];
  dir2 = 0; pwm2 = M3[angle-120];
  dir3 = 1; pwm3 = M1[angle-120];
  }
else if (angle >= 180 && angle < 240){
  dir1 = 0; pwm1 = M1[angle-180];
  dir2 = 0; pwm2 = M2[angle-180];
  dir3 = 1; pwm3 = M3[angle-180];
  }
else if (angle >=240 && angle < 300){
  dir1 = 1; pwm1 = M3[angle-240];
  dir2 = 0; pwm2 = M1[angle-240];
  dir3 = 0; pwm3 = M2[angle-240];
  }
else if (angle >= 300 && angle < 360){
  dir1 = 0; pwm1 = M2[angle-300];
  dir2 = 1; pwm2 = M3[angle-300];
  dir3 = 0; pwm3 = M1[angle-300];
  }
//      Serial.print("Angle is:\t");
//      Serial.println(angle);      
//Serial.print(F("\tRightHatX: "));
//Serial.print(modHatValue(PS4.getAnalogHat(RightHatX)));
//Serial.print(F("\tRightHatY: "));
//Serial.print(modHatValue(PS4.getAnalogHat(RightHatY)));
  analogWrite(PWM_M1, pwm1);  digitalWrite(DIR_M1, dir1);
  analogWrite(PWM_M2, pwm2);  digitalWrite(DIR_M2, dir2);
  analogWrite(PWM_M3, pwm3);  digitalWrite(DIR_M3, dir3);
//  analogWrite(PWM_M4, pwm4);  digitalWrite(DIR_M4, dir4); 
  digitalWrite(ENABLE_MOTORS, LOW);  // enable = LOW
}

if (PS4.getAnalogButton(L2) || PS4.getAnalogButton(R2)) { // These are the only analog buttons on the PS4 controller
//  Serial.print(F("\r\nL2: "));
//  Serial.print(PS4.getAnalogButton(L2));  
//  Serial.print(F("\tR2: "));
//  Serial.print(PS4.getAnalogButton(R2));
  
  if (PS4.getAnalogButton(L2)>15){
  dir1 = 0; pwm1 = PS4.getAnalogButton(L2);
  dir2 = 0; pwm2 = PS4.getAnalogButton(L2);
  dir3 = 0; pwm3 = PS4.getAnalogButton(L2);
  }
  else if (PS4.getAnalogButton(R2)>15){
  dir1 = 1; pwm1 = PS4.getAnalogButton(R2);
  dir2 = 1; pwm2 = PS4.getAnalogButton(R2);
  dir3 = 1; pwm3 = PS4.getAnalogButton(R2);
  }
    analogWrite(PWM_M1, pwm1);  digitalWrite(DIR_M1, dir1);
  analogWrite(PWM_M2, pwm2);  digitalWrite(DIR_M2, dir2);
  analogWrite(PWM_M3, pwm3);  digitalWrite(DIR_M3, dir3);
//  analogWrite(PWM_M4, pwm4);  digitalWrite(DIR_M4, dir4); 
  digitalWrite(ENABLE_MOTORS, LOW);  // enable = LOW
}
if (PS4.getButtonClick(PS)) {
  Serial.print(F("\r\nPS"));
  PS4.disconnect(); 
}
else {
  if (PS4.getButtonClick(TRIANGLE))
    Serial.print(F("\r\nTraingle"));
  if (PS4.getButtonClick(CIRCLE))
    Serial.print(F("\r\nCircle"));
  if (PS4.getButtonClick(CROSS))
    Serial.print(F("\r\nCross"));
  if (PS4.getButtonClick(SQUARE))
    Serial.print(F("\r\nSquare"));

  if (PS4.getButtonClick(UP))
    Serial.print(F("\r\nUp"));
  if (PS4.getButtonClick(RIGHT))
    Serial.print(F("\r\nRight"));
  if (PS4.getButtonClick(DOWN))
    Serial.print(F("\r\nDown"));
  if (PS4.getButtonClick(LEFT))
    Serial.print(F("\r\nLeft"));

  if (PS4.getButtonClick(L1))
    Serial.print(F("\r\nL1"));
  if (PS4.getButtonClick(L3))
    Serial.print(F("\r\nL3"));
  if (PS4.getButtonClick(R1))
    Serial.print(F("\r\nR1"));
  if (PS4.getButtonClick(R3))
    Serial.print(F("\r\nR3"));

  if (PS4.getButtonClick(SHARE))
    Serial.print(F("\r\nShare"));
  if (PS4.getButtonClick(OPTIONS)) {
    Serial.print(F("\r\nOptions"));
    printAngle = !printAngle;
  }
  if (PS4.getButtonClick(TOUCHPAD)) {
    Serial.print(F("\r\nTouchpad"));
    printTouch = !printTouch;
  }

  if (printAngle) { // Print angle calculated using the accelerometer only
    Serial.print(F("\r\nPitch: "));
    Serial.print(PS4.getAngle(Pitch));
    Serial.print(F("\tRoll: "));
    Serial.print(PS4.getAngle(Roll));
  }

  if (printTouch) { // Print the x, y coordinates of the touchpad
    if (PS4.isTouching(0) || PS4.isTouching(1)) // Print newline and carriage return if any of the fingers are touching the touchpad
      Serial.print(F("\r\n"));
    for (uint8_t i = 0; i < 2; i++) { // The touchpad track two fingers
      if (PS4.isTouching(i)) { // Print the position of the finger if it is touching the touchpad
        Serial.print(F("X")); Serial.print(i + 1); Serial.print(F(": "));
        Serial.print(PS4.getX(i));
        Serial.print(F("\tY")); Serial.print(i + 1); Serial.print(F(": "));
        Serial.print(PS4.getY(i));
        Serial.print(F("\t"));
      }
    }
  }
}
}
}









void setPwmFrequency(int pin, int divisor)
{  
  byte mode;
  if(pin == 11 || pin == 12 || pin == 9 || pin == 10 ||
     pin == 2 || pin == 3 || pin == 5 || pin == 6 ||
     pin == 7 || pin == 8 || pin == 44 || pin == 45 || pin == 46)  //Timers1-5
    {
      switch(divisor) 
      {
      case 1: mode = 0x01; break;
      case 2: mode = 0x02; break;
      case 3: mode = 0x03; break;
      case 4: mode = 0x04; break;
      case 5: mode = 0x05; break;
      default: return;
      }
    
      if(pin == 11 || pin == 12)
      { 
      TCCR1B = TCCR1B & 0b11111000 | mode; // Sets Timer1
      } 
      else if(pin == 9 || pin == 10)
      {
      TCCR2B = TCCR2B & 0b11111000 | mode; // Sets Timer2
      }
      else if(pin == 2 || pin == 3 || pin == 5)
      {
      TCCR3B = TCCR3B & 0b11111000 | mode; // Sets Timer3
      }
      else if(pin == 6 || pin ==7 || pin == 8)
      {
      TCCR4B = TCCR4B & 0b11111000 | mode; // Sets Timer4
      }
      else 
      {
      TCCR5B = TCCR5B & 0b11111000 | mode; //Sets Timer 5
      }
    }
  else if(pin == 4 || pin == 13)      //Timer0
   {
      switch(divisor) 
      {
      case 1: mode = 0x01; break;
      case 2: mode = 0x02; break;
      case 3: mode = 0x03; break;
      case 4: mode = 0x04; break;
      case 5: mode = 0x05; break;
      default: return;
      }
      TCCR0B = TCCR0B & 0b11111000 | mode; // Sets Timer0
   }
}

//Sets the desired range of output for Hat values from -255-0, and 0-255
int modHatValue(int x){
  int modval;
  if (x > 138){
      modval = 2*x-255;    
      }
  else if (x < 118){
      modval = 2*x-255;    
      }
  else {
    modval = 0;
         }
   return modval;       //Returns the Modified Hat Value
  }


//Calculates the Velocity magnitude
  long calcVelocity (long x, long y){
    long tempvelocity;                           
    tempvelocity = sqrt(x*x+y*y);               //Takes x and y and calulated the polar magnitude
    if (tempvelocity>255){                      //Sets velocity max to 255
      tempvelocity = 255;
    }
    return tempvelocity;
    }


//rounds float or double to nearest integer
int roundint (double r) {
  int tmp = static_cast<int> (r);
  tmp += (r-tmp>=.5) - (r-tmp<=-.5);
  return tmp;
}



