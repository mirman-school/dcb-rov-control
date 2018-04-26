#include <AFMotor.h>

// CONSTANTS
const int JOY_MIN = 0; // Min val from joystick read
const int JOY_MAX = 1023; // Max val from joystick read
const int MOTOR_MIN = 0; // Min val to motor control
const int MOTOR_MAX = 255; // Max val to motor control


// DIRECTION CONSTANTS
// We do this so we don't risk a typo later
const int AHEAD = 1;
const int BACK = 2;
const int PORT = 3;
const int STARBOARD = 4;
const int AHEAD_PORT = 5;
const int BACK_PORT = 6;
const int AHEAD_STARBOARD = 7;
const int BACK_STARBOARD = 8;
const int STOP = 9;

// VARIABLES
int stickX; // joystick x val
int stickY; // joystick y val
int stickZ; // joystick z val
int vecX; // translated x val to motors
int vecY; // translated y val to motors
int vecZ; // translated z val to motors
int dir; // direction command
int nullX; // x axis null zone
int nullY; // y axis stick null zone
int nullZ; // z axis
int speed1; // Motor 1 speed
int speed2; // Motor 2 speed
int speed3; // Motor 3 speed

// MOTOR DEFINITIONS
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);


void setup() {
  // Set up serial port to print inputs and outputs
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  nullX = analogRead(A1);
  nullY = analogRead(A0);
  nullZ = analogRead(A2);
}
/*
  loop() runs forever once the Arduino has started. Consider this the
  "life" of your device.
*/
void loop() {

  // Get stickX/stickY joystick values
  stickX = analogRead(A1);
  stickY = analogRead(A0);
  stickZ = analogRead(A2);

  // Turn those into a direction
  dir = resolveDirection(stickX, stickY);
  vecX = mapStickVal(stickX, nullX);
  vecY = mapStickVal(stickY, nullY);
  vecZ = mapStickVal(stickZ, nullZ);

  // Control structure for x/y directions
  switch (dir) {
    case STOP:
      speed1 = vecY;
      speed2 = vecY;
      break;
    case AHEAD:
      speed1 = vecY;
      speed2 = vecY;
      break;
    case BACK:
      speed1 = vecY;
      speed2 = vecY;
      break;
    case PORT:
      speed1 = vecY;
      speed2 = vecX;
      break;
    case STARBOARD:
      speed1 = vecX;
      speed2 = vecY;
      break;
    case AHEAD_PORT:
      speed1 = vecY - vecX;
      speed2 = vecX;
      break;
    case AHEAD_STARBOARD:
      speed1 = vecX;
      speed2 = vecY - vecX;
      break;
    case BACK_PORT:
      speed1 = vecY - vecX;
      speed2 = vecX;
      break;
    case BACK_STARBOARD:
      speed1 = vecX;
      speed2 = vecY - vecX;
      break;
    default:
      speed1 = 0;
      speed2 = 0;

      speed3 = vecZ;
  }

  // Serial print the results;
  Serial.print(stickX);
  Serial.print("/");
  Serial.print(stickY);
  Serial.print("/");
  Serial.print(stickZ);
  Serial.print("::");
  Serial.print(vecX);
  Serial.print("/");
  Serial.print(vecY);
  Serial.print("/");
  Serial.print(vecZ);
  Serial.print("::");
  Serial.print(speed1);
  Serial.print("/");
  Serial.print(speed2);
  Serial.print("/");
  Serial.print(speed3);
  Serial.print("::");
  Serial.print(dir);
  Serial.println("");

  // Do the actual motor update
  motor1.setSpeed(speed1);
  motor2.setSpeed(speed2);
  motor3.setSpeed(speed3);

  if (stickX < nullX) {
    motor1.run(BACKWARD);
    motor2.run(BACKWARD);
  } else {
    motor1.run(FORWARD);
    motor2.run(FORWARD);
  }

  if (stickZ < nullZ) {
    motor3.run(BACKWARD);
  } else if (stickZ > nullZ)  {
    motor3.run(FORWARD);
  } else {
    motor3.run(RELEASE);
  }


  delay(500);
}

int resolveDirection(int x, int y) {

  // STOP
  if (x == nullX && y == nullY) {
    return STOP;
  }

  // AHEAD
  if (x == nullX && y > nullY) {
    return AHEAD;
  }

  // BACK
  if (x == nullX && y < nullY) {
    return BACK;
  }

  // PORT
  if (x < nullX && y == nullY) {
    return PORT;
  }

  // STARBOARD
  if (x > nullX && y == nullY) {
    return STARBOARD;
  }

  // AHEAD_PORT
  if (x < nullX && y > nullY) {
    return AHEAD_PORT;
  }

  // BACK_PORT
  if (x < nullX && y < nullY) {
    return BACK_PORT;
  }

  // AHEAD_STARBOARD
  if (x > nullX && y > nullY) {
    return AHEAD_STARBOARD;
  }

  // BACK_STARBOARD
  if (x > nullX && y < nullY) {
    return BACK_STARBOARD;
  }

}

int mapStickVal(int stickVal, int nullVal) {
  if (stickVal == nullVal) {
    return MOTOR_MIN;
  }

  if (stickVal <= nullVal) {
    return map(JOY_MAX - stickVal, JOY_MIN, JOY_MAX, MOTOR_MIN, MOTOR_MAX);
  }
  return map(stickVal, JOY_MIN, JOY_MAX, MOTOR_MIN, MOTOR_MAX);
}
