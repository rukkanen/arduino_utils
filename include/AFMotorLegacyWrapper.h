#ifndef AFMOTORLEGACYWRAPPER_H
#define AFMOTORLEGACYWRAPPER_H

#ifndef ESP8266

#include <AFMotor.h>

class AFMotorLegacyWrapper
{
private:
  AF_DCMotor motors[4];
  int currentSpeed[4];
  int targetSpeed[4];
  int accelerationStep;
  unsigned long accelerationCycleLength;
  int irPin[4];
  static volatile unsigned long encoderCount[4]; // Declare encoderCount here

  static void encoderISR0(); // Declare ISRs
  static void encoderISR1();
  static void encoderISR2();
  static void encoderISR3();

public:
  // Constructor
  AFMotorLegacyWrapper(int motorPins[4], int irPins[4]);

  // Set target speeds
  void setTargetSpeeds(int speed0, int speed1, int speed2, int speed3);

  // Update speeds gradually
  void updateSpeed();

  // Basic movements
  void driveForward(int speed);
  void driveBackward(int speed);
  void rotateClockwise(int speed);
  void rotateCounterClockwise(int speed);

  // Curving by adjusting speeds
  void curve(int leftSpeed, int rightSpeed);

  // Measure distance traveled
  unsigned long measureDistance();

  // Acceleration profiles
  void setFastAcceleration();
  void setMediumAcceleration();
  void setSlowAcceleration();
};

#endif
#endif
