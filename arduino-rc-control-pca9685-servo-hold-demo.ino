/*
  RC control servo hold example
  rc-control-pca9685-servo-hold-demo.ino
  Demonstrates how to use an RC controller like the FlySky FS-i6 or similar to control servos,
  while holding the servo position even if the joystick centers.

  PCA9685 code taken from:
  -----------------------------------------
  PCA9685 PWM Servo Driver Example
  pca9685-servomotor-demo.ino
  Demonstrates use of 16 channel I2C PWM driver board with 4 servo motors
  Uses Adafruit PWM library
  Uses 4 potentiometers for input

  DroneBot Workshop 2018
  https://dronebotworkshop.com
  -----------------------------------------
*/

// Include Wire Library for I2C Communications
#include <Wire.h>

// Include Adafruit PWM Library
#include <Adafruit_PWMServoDriver.h>

#define MIN_PULSE_WIDTH       650
#define MAX_PULSE_WIDTH       2350
#define FREQUENCY             50

// Define Input Connections
#define CH1 3
#define CH2 5
//#define CH3 6
//#define CH4 9
//#define CH5 10
//#define CH6 11

// Integers to represent values from sticks and pots
int ch1Value;
int ch2Value;
//int ch3Value;
//int ch4Value;
//int ch5Value;

// Boolean to represent switch value
//bool ch6Value;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();



// Define Motor Outputs on PCA9685 board

int motorA = 0;
int motorB = 4;
int motorC = 8;
int motorD = 12;

void setup()
{
  // Set up serial monitor
  Serial.begin(115200);

  // Set all pins as inputs
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  //  pinMode(CH3, INPUT);
  //  pinMode(CH4, INPUT);
  //  pinMode(CH5, INPUT);
  //  pinMode(CH6, INPUT);

  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);
}

float pulse_wide1 = -1000000;

void moveMotor1(int controlIn)
{
  int localcontrol = controlIn;
  if (localcontrol < 20 && localcontrol > -20)
  {
    localcontrol = 0;
  }

  float pulse_wide, pulse_width;

  // Convert to pulse width
  if (pulse_wide1 <= -1000000)
  {
    pulse_wide = map(localcontrol, -100, 100, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  }
  else
  {
    pulse_wide = pulse_wide1 + float(localcontrol) / 10;
    if (pulse_wide < MIN_PULSE_WIDTH)
    {
      pulse_wide = MIN_PULSE_WIDTH;
    }
    else if (pulse_wide > MAX_PULSE_WIDTH)
    {
      pulse_wide = MAX_PULSE_WIDTH;
    }
  }
  pulse_width = int(pulse_wide / 1000000 * FREQUENCY * 4096);

Serial.print(" | Control: ");
Serial.print(localcontrol);
  Serial.print(" | PW1: ");
  Serial.print(pulse_wide1);
  Serial.print(" | PW: ");
  Serial.print(pulse_wide);


  //Control Motor
  pwm.setPWM(motorA, 0, pulse_width);
  pulse_wide1 = pulse_wide;
}

void moveMotor2(int controlIn)
{
  int pulse_wide, pulse_width;

  // Convert to pulse width
  pulse_wide = map(controlIn, -100, 100, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  pulse_width = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);

  Serial.print(" | PW: ");
  Serial.print(pulse_wide);
  //Control Motor
  pwm.setPWM(motorB, 0, pulse_width);
}

void loop() {
  // Get values for each channel
  ch1Value = readChannel(CH1, -100, 100, 0);
  Serial.print("Ch1: ");
  Serial.print(ch1Value);
  //Control Motor A
  moveMotor1(ch1Value);

  //  ch2Value = readChannel(CH2, -100, 100, 0);
  //  Serial.print(" | Ch2: ");
  //  Serial.print(ch2Value);
  //  moveMotor2(ch2Value);
  Serial.println();


  // Print to Serial Monitor




  //  delay(10);
}


/*
  Arduino FS-I6X Demo
  fsi6x-arduino-uno.ino
  Read output ports from FS-IA6B receiver module
  Display values on Serial Monitor

  Channel functions by Ricardo Paiva - https://gist.github.com/werneckpaiva/

  DroneBot Workshop 2021
  https://dronebotworkshop.com
*/

// Read the number of a specified channel and convert to the range provided.
// If the channel is off, return the default value
int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue) {
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}
