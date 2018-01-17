#include <AFMotor.h>
/* 
-------------------------------
The MIT License (MIT)
Copyright (c) 2014 Blue Robotics Inc.
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-------------------------------*/

// Joystick Settings
int JS_CENTER_X; // Analog reading at center, 0-1023
int JS_CENTER_Y; // Analog reading at center, 0-1023
static const int JS_RANGE_X = 128; // Analog range, 0-1023
static const int JS_RANGE_Y = 128; // Set to 128 for Parallax joystick
static const int JS_DIR_X = 1; // +1 or -1
static const int JS_DIR_Y = 1;

// Thruster Settings
static const int MAX_FWD_REV_THROTTLE = 255; // Value between 0-255
static const int MAX_TURN_THROTTLE = 255; // Value between 0-255

// Arduino Pins
static const byte JS_ADC_0 = A0;
static const byte JS_ADC_1 = A1;

// MOTOR DEFINITIONS
AF_DCMotor motor1(1); 
AF_DCMotor motor2(2);


void setup() {
    // Set up serial port to print inputs and outputs
    Serial.begin(9600);
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);

    // Get initial positions for center of JS
    JS_CENTER_X = analogRead(A0);
    JS_CENTER_Y = analogRead(A1);

    delay(1000); 
}

void loop() {
    // Read the joysticks and use the Arduino "map" function to map the raw values
    // to the desired output commands.
   
    int jsX = analogRead(JS_ADC_0);
    int jsY = analogRead(JS_ADC_1);

    int forwardCommand    = map(analogRead(JS_ADC_0), // Read raw joystick value
                              JS_CENTER_Y-JS_DIR_Y*JS_RANGE_Y, // Joystick low value
                              JS_CENTER_Y+JS_DIR_Y*JS_RANGE_Y, // Joystick high value
                              -MAX_FWD_REV_THROTTLE, // Command low value
                              MAX_FWD_REV_THROTTLE); // Command high value

    int turnCommand       = map(analogRead(JS_ADC_1), // Read raw joystick value
                              JS_CENTER_X-JS_DIR_X*JS_RANGE_X, // Joystick low value
                              JS_CENTER_X+JS_DIR_X*JS_RANGE_X, // Joystick high value
                              -MAX_TURN_THROTTLE, // Command low value
                              MAX_TURN_THROTTLE); // Command high value

    // Set motor direction
    if (forwardCommand < 0) {
        motor1.run(BACKWARD);
        motor2.run(BACKWARD);
    } else {
        motor1.run(FORWARD);
        motor2.run(FORWARD);
    }

    // Combine the "stopped" command with forward, turn, and vertical and send 
    // to the Thrusters.
    motor1.setSpeed(forwardCommand+turnCommand);
    motor2.setSpeed(forwardCommand-turnCommand);

    // Output via serial
    Serial.print("X: ");
    Serial.print(jsX);
    Serial.print("Y: ");
    Serial.print(jsY);
    Serial.print("/");
    Serial.print("Fwd: "); Serial.print(forwardCommand);
    Serial.print("Turn: "); Serial.print(turnCommand);
    Serial.println("");

    // Delay 1/10th of a second. No need to update at super fast rates.
    delay(100);
}
