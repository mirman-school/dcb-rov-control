#include <AFMotor.h>
// CONSTANTS
static const int JOY_CENTER = 511;  // Center val for joystick
static const int JOY_MIN = 0; // Min val from joystick read
static const int JOY_MAX = 0; // Max val from joystick read
static const int MOTOR_MIN = 0; // Min val to motor control
static const int MOTOR_MAX = 255; // Max val to motor control
static const int NULL_MIN = 506; // Low end stick null zone
static const int NULL_MAX = 516; // High end stick null zone

// DIRECTION CONSTANTS
// We do this so we don't risk a typo later
static const String AHEAD = "AHEAD";
static const String BACK = "BACK";
static const String PORT = "PORT";
static const String STARBOARD = "STARBOARD";
static const String AHEAD_PORT = "AHEAD_PORT";
static const String BACK_PORT = "BACK_PORT";
static const String AHEAD_STARBOARD = "AHEAD_STARBOARD";
static const String BACK_STARBOARD = "BACK_STARBOARD";
static const String STOP = "STOP";

// VARIABLES
int stickX; // joystick x val
int stickY; // joystick y val
int vecX; // translated x val to motors
int vecY; // translated y val to motors
String dir; // direction command

// MOTOR DEFINITIONS
AF_DCMotor motor1(1); 
AF_DCMotor motor2(2);

/*
setup() runs when the Arduino turns on. We put any code needed to
initialize the program here.
*/
void setup() {
    // Set up joystick pins for input
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    
    // Begin serial printing for the run
    Serial.begin(9600);
}

/*
loop() runs forever once the Arduino has started. Consider this the
"life" of your device.
*/
void loop() {
  
    // Get stickX/stickY joystick values
    stickX = nullify(analogRead(A0));
    stickY = nullify(analogRead(A1));
    dir = resolveDirection(stickX, stickY);

    switch (dir) {
        case STOP:
            stop();
            break;
        case AHEAD:
            ahead();
            break;
        case BACK:
            back();
            break;
        case PORT:
            port();
            break;
        case STARBOARD:
            starboard();
            break;
        case AHEAD_PORT:
            aheadPort();
            break;
        case AHEAD_STARBOARD:
            aheadStarboard();
            break;
        case BACK_PORT:
            backPort();
            break;
        case BACK_STARBOARD:
            backStarboard();
            break;
        default:
            stop();
    }

}

int nullify(int stickVal) {
    if (stickVal >= NULL_MIN && stickVal <= NULL_MAX) {
        return JOY_CENTER; 
    }
}

String resolveDirection(int x, y) {
    
    // STOP
    if (x == JOY_CENTER && y == JOY_CENTER) {
        return STOP;
    }

    // AHEAD
    if (x == JOY_CENTER && y > JOY_CENTER) {
        return AHEAD;
    }

    // BACK
    if (x == JOY_CENTER && y < JOY_CENTER) {
        return BACK;
    }

    // PORT
    if (x < JOY_CENTER && y == JOY_CENTER) {
        return PORT;
    }

    // STARBOARD
    if (x > JOY_CENTER && y == JOY_CENTER) {
        return STARBOARD;
    }

    // AHEAD_PORT
    if (x < JOY_CENTER && y > JOY_CENTER) {
        return AHEAD_PORT; 
    }

    // BACK_PORT
    if (x < JOY_CENTER && y < JOY_CENTER) {
        return BACK_PORT; 
    }

    // AHEAD_STARBOARD
    if (x > JOY_CENTER && y > JOY_CENTER) {
        return AHEAD_STARBOARD; 
    }

    // BACK_STARBOARD
    if (x > JOY_CENTER && y < JOY_CENTER) {
        return BACK_STARBOARD; 
    }

    return STOP;
}

void ahead() {
    vecX = 0;
    vecY = map(stickY, JOY_MIN, JOY_MAX, MOTOR_MIN, MOTOR_MAX);
    motor1.setSpeed(vecY);
    motor2.setSpeed(vecY);
    motor1.run(FORWARD);
    motor1.run(FORWARD);
}

void back() {
    vecX = 0;
    vecY = mapStickVal(JOY_MAX - stickY)
    motor1.setSpeed(vecY);
    motor2.setSpeed(vecY);
    motor1.run(BACKWARD);
    motor1.run(BACKWARD);
}

void port() {
    vecX = mapStickVal(JOY_MAX - stickX);
    vecY = 0;
    motor1.setSpeed(vecY);
    motor2.setSpeed(vecX);
    motor1.run(FORWARD);
    motor2.run(FORWARD);
}

void starboard() {
    vecX = mapStickVal(stickX);
    vecY = 0;
    motor1.setSpeed(vecX);
    motor2.setSpeed(vecY);
    motor1.run(FORWARD);
    motor2.run(FORWARD);
}

void aheadPort() {
    vecX = mapStickVal(JOY_MAX - stickX);
    vecY = mapStickVal(stickY);
    motor1.setSpeed(vecY - vecX);
    motor1.setSpeed(vecX);
    motor1.run(FORWARD);
    motor1.run(FORWARD);
}

void aheadStarboard() {
    vecX = mapStickVal(stickX);
    vecY = mapStickVal(stickY);
    motor1.setSpeed(vecX);
    motor2.setSpeed(vecY - vecX);
    motor1.run(FORWARD);
    motor2.run(FORWARD);

}

void backPort() {
    vecX = mapStickVal(JOY_MAX - stickX);
    vecY = mapStickVal(JOY_MAX - stickY);
    motor1.setSpeed(vecY - vecX);
    motor2.setSpeed(vecX);
    motor1.run(BACKWARD);
    motor2.run(BACKWARD)

}

void backStarboard() {
    vecX = mapStickVal(stickX);
    vecY = mapStickVal(JOY_MAX - stickY);
    motor1.setSpeed(vecX);
    motor2.setSpeed(vecY - vecX);
    motor1.run(BACKWARD);
    motor2.run(BACKWARD)
}

int mapStickVal(int stickVal) {
    return map(stickVal, JOY_MIN, JOY_MAX, MOTOR_MIN, MOTOR_MAX);
}
