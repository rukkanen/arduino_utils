#include "AFMotorLegacyWrapper.h"
#include <Arduino.h>

// Static variables for encoder counts
volatile unsigned long AFMotorLegacyWrapper::encoderCount[4] = {0, 0, 0, 0};

// Constructor
AFMotorLegacyWrapper::AFMotorLegacyWrapper(int motorPins[4], int irPins[4])
    : motors{AF_DCMotor(motorPins[0]), AF_DCMotor(motorPins[1]),
             AF_DCMotor(motorPins[2]), AF_DCMotor(motorPins[3])}
{
  for (int i = 0; i < 4; i++)
  {
    currentSpeed[i] = 0;
    targetSpeed[i] = 0;
    irPin[i] = irPins[i];
    encoderCount[i] = 0;
    pinMode(irPin[i], INPUT);
  }

  attachInterrupt(digitalPinToInterrupt(irPin[0]), encoderISR0, RISING);
  attachInterrupt(digitalPinToInterrupt(irPin[1]), encoderISR1, RISING);
  attachInterrupt(digitalPinToInterrupt(irPin[2]), encoderISR2, RISING);
  attachInterrupt(digitalPinToInterrupt(irPin[3]), encoderISR3, RISING);
}

// Interrupt Service Routines (ISRs)
void IRAM_ATTR AFMotorLegacyWrapper::encoderISR0() { encoderCount[0]++; }
void IRAM_ATTR AFMotorLegacyWrapper::encoderISR1() { encoderCount[1]++; }
void IRAM_ATTR AFMotorLegacyWrapper::encoderISR2() { encoderCount[2]++; }
void IRAM_ATTR AFMotorLegacyWrapper::encoderISR3() { encoderCount[3]++; }

// Set target speeds
void AFMotorLegacyWrapper::setTargetSpeeds(int speed0, int speed1, int speed2, int speed3)
{
  targetSpeed[0] = speed0;
  targetSpeed[1] = speed1;
  targetSpeed[2] = speed2;
  targetSpeed[3] = speed3;
}

// Update speeds gradually
void AFMotorLegacyWrapper::updateSpeed()
{
  for (int i = 0; i < 4; i++)
  {
    if (currentSpeed[i] < targetSpeed[i])
    {
      currentSpeed[i] += accelerationStep;
      if (currentSpeed[i] > targetSpeed[i])
      {
        currentSpeed[i] = targetSpeed[i];
      }
    }
    else if (currentSpeed[i] > targetSpeed[i])
    {
      currentSpeed[i] -= accelerationStep;
      if (currentSpeed[i] < targetSpeed[i])
      {
        currentSpeed[i] = targetSpeed[i];
      }
    }
    motors[i].setSpeed(currentSpeed[i]);
  }
}

// Drive forward
void AFMotorLegacyWrapper::driveForward(int speed)
{
  setTargetSpeeds(speed, speed, speed, speed);
  updateSpeed();
  for (int i = 0; i < 4; i++)
  {
    motors[i].run(FORWARD);
  }
}

// Drive backward
void AFMotorLegacyWrapper::driveBackward(int speed)
{
  setTargetSpeeds(speed, speed, speed, speed);
  updateSpeed();
  for (int i = 0; i < 4; i++)
  {
    motors[i].run(BACKWARD);
  }
}

// Rotate clockwise
void AFMotorLegacyWrapper::rotateClockwise(int speed)
{
  setTargetSpeeds(speed, speed, speed, speed);
  updateSpeed();
  motors[0].run(FORWARD);
  motors[1].run(BACKWARD);
  motors[2].run(FORWARD);
  motors[3].run(BACKWARD);
}

// Rotate counterclockwise
void AFMotorLegacyWrapper::rotateCounterClockwise(int speed)
{
  setTargetSpeeds(speed, speed, speed, speed);
  updateSpeed();
  motors[0].run(BACKWARD);
  motors[1].run(FORWARD);
  motors[2].run(BACKWARD);
  motors[3].run(FORWARD);
}

// Curving by adjusting speeds
void AFMotorLegacyWrapper::curve(int leftSpeed, int rightSpeed)
{
  setTargetSpeeds(leftSpeed, leftSpeed, rightSpeed, rightSpeed);
  updateSpeed();
  motors[0].run(FORWARD);
  motors[1].run(FORWARD);
  motors[2].run(FORWARD);
  motors[3].run(FORWARD);
}

// Measure distance traveled
unsigned long AFMotorLegacyWrapper::measureDistance()
{
  // Average the count from all four encoders for more precise measurement
  unsigned long distance = (encoderCount[0] + encoderCount[1] + encoderCount[2] + encoderCount[3]) / 4;
  // Reset counts if needed
  for (int i = 0; i < 4; i++)
  {
    encoderCount[i] = 0;
  }
  return distance;
}

// Acceleration profiles
void AFMotorLegacyWrapper::setFastAcceleration()
{
  accelerationStep = 10;
  accelerationCycleLength = 20; // in milliseconds
}

void AFMotorLegacyWrapper::setMediumAcceleration()
{
  accelerationStep = 5;
  accelerationCycleLength = 50;
}

void AFMotorLegacyWrapper::setSlowAcceleration()
{
  accelerationStep = 2;
  accelerationCycleLength = 100;
}

#include "AFMotorLegacyWrapper.h"

// Static variables for encoder counts
volatile unsigned long AFMotorLegacyWrapper::encoderCount[4] = {0, 0, 0, 0};

// Constructor
AFMotorLegacyWrapper::AFMotorLegacyWrapper(int motorPins[4], int irPins[4])
    : motors{AF_DCMotor(motorPins[0]), AF_DCMotor(motorPins[1]),
             AF_DCMotor(motorPins[2]), AF_DCMotor(motorPins[3])}
{
  for (int i = 0; i < 4; i++)
  {
    currentSpeed[i] = 0;
    targetSpeed[i] = 0;
    irPin[i] = irPins[i];
    encoderCount[i] = 0;
    pinMode(irPin[i], INPUT);
  }

  attachInterrupt(digitalPinToInterrupt(irPin[0]), encoderISR0, RISING);
  attachInterrupt(digitalPinToInterrupt(irPin[1]), encoderISR1, RISING);
  attachInterrupt(digitalPinToInterrupt(irPin[2]), encoderISR2, RISING);
  attachInterrupt(digitalPinToInterrupt(irPin[3]), encoderISR3, RISING);
}

// Interrupt Service Routines (ISRs)
void IRAM_ATTR AFMotorLegacyWrapper::encoderISR0() { encoderCount[0]++; }
void IRAM_ATTR AFMotorLegacyWrapper::encoderISR1() { encoderCount[1]++; }
void IRAM_ATTR AFMotorLegacyWrapper::encoderISR2() { encoderCount[2]++; }
void IRAM_ATTR AFMotorLegacyWrapper::encoderISR3() { encoderCount[3]++; }

// Set target speeds
void AFMotorLegacyWrapper::setTargetSpeeds(int speed0, int speed1, int speed2, int speed3)
{
  targetSpeed[0] = speed0;
  targetSpeed[1] = speed1;
  targetSpeed[2] = speed2;
  targetSpeed[3] = speed3;
}

// Update speeds gradually
void AFMotorLegacyWrapper::updateSpeed()
{
  for (int i = 0; i < 4; i++)
  {
    if (currentSpeed[i] < targetSpeed[i])
    {
      currentSpeed[i] += accelerationStep;
      if (currentSpeed[i] > targetSpeed[i])
      {
        currentSpeed[i] = targetSpeed[i];
      }
    }
    else if (currentSpeed[i] > targetSpeed[i])
    {
      currentSpeed[i] -= accelerationStep;
      if (currentSpeed[i] < targetSpeed[i])
      {
        currentSpeed[i] = targetSpeed[i];
      }
    }
    motors[i].setSpeed(currentSpeed[i]);
  }
}

// Drive forward
void AFMotorLegacyWrapper::driveForward(int speed)
{
  setTargetSpeeds(speed, speed, speed, speed);
  updateSpeed();
  for (int i = 0; i < 4; i++)
  {
    motors[i].run(FORWARD);
  }
}

// Drive backward
void AFMotorLegacyWrapper::driveBackward(int speed)
{
  setTargetSpeeds(speed, speed, speed, speed);
  updateSpeed();
  for (int i = 0; i < 4; i++)
  {
    motors[i].run(BACKWARD);
  }
}

// Rotate clockwise
void AFMotorLegacyWrapper::rotateClockwise(int speed)
{
  setTargetSpeeds(speed, speed, speed, speed);
  updateSpeed();
  motors[0].run(FORWARD);
  motors[1].run(BACKWARD);
  motors[2].run(FORWARD);
  motors[3].run(BACKWARD);
}

// Rotate counterclockwise
void AFMotorLegacyWrapper::rotateCounterClockwise(int speed)
{
  setTargetSpeeds(speed, speed, speed, speed);
  updateSpeed();
  motors[0].run(BACKWARD);
  motors[1].run(FORWARD);
  motors[2].run(BACKWARD);
  motors[3].run(FORWARD);
}

// Curving by adjusting speeds
void AFMotorLegacyWrapper::curve(int leftSpeed, int rightSpeed)
{
  setTargetSpeeds(leftSpeed, leftSpeed, rightSpeed, rightSpeed);
  updateSpeed();
  motors[0].run(FORWARD);
  motors[1].run(FORWARD);
  motors[2].run(FORWARD);
  motors[3].run(FORWARD);
}

// Measure distance traveled
unsigned long AFMotorLegacyWrapper::measureDistance()
{
  // Average the count from all four encoders for more precise measurement
  unsigned long distance = (encoderCount[0] + encoderCount[1] + encoderCount[2] + encoderCount[3]) / 4;
  // Reset counts if needed
  for (int i = 0; i < 4; i++)
  {
    encoderCount[i] = 0;
  }
  return distance;
}

// Acceleration profiles
void AFMotorLegacyWrapper::setFastAcceleration()
{
  accelerationStep = 10;
  accelerationCycleLength = 20; // in milliseconds
}

void AFMotorLegacyWrapper::setMediumAcceleration()
{
  accelerationStep = 5;
  accelerationCycleLength = 50;
}

void AFMotorLegacyWrapper::setSlowAcceleration()
{
  accelerationStep = 2;
  accelerationCycleLength = 100;
}
