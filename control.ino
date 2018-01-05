#include <AFMotor.h>
int stickX; // motor 1 joystick
int stickY; // motor 2 joystick
int vecX; // normalized motor rate
int vecY; // normalized motor rate

/*
The null zone gives a little more zero for stop.
*/
int nullMin = 490;
int nullMax = 510;

/* 
Software definitions for both motors so we can control them
*/
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
    stickX = analogRead(A0);
    stickY = analogRead(A1);

    // Turn raw stick input into vectors for the motors
    vecX = normalize(stickX, 0, 1023, 0, 255);
    vecY = normalize(stickY, 0, 1023, 0, 255);

    // Steering instructions
    if (vecX == 0 && vecY == 0) {
        stop();  
    } else if (vecX == 0) {
        if (vecY >= 128 ) {
            forward();
        } else {
            reverse();
        }
    } else {
        turn();
    }
    
    // Print the data from the device
    Serial.print(stickX);
    Serial.print('-');
    Serial.println(stickY);
    Serial.print(vecX);
    Serial.print('/');
    Serial.print(vecY);
}

int normalize(int stick) {
    /*
    The stick has ranges 0-1023
    
       1023
        |
    0-------1023
        |
        0

    But we need to consider all back as full power, and all left/right as full power. So like:

         255
          |
    255---0---255
          |
         255

    */

    if (stick >= nullMin && stick <= nullMax) {
        return 0;
    } else if (stick < 512) {
        return map(stick, 0, 511, 0, 255);
    } else {
        return map(stick, 512, 1023, 0, 255);
    }
}

/*
Helper functions for managing the motors
*/

void stop() {
    motor1.setSpeed(0);
    motor1.setSpeed(0);
}


void forward() {
    motor1.setSpeed(vecY);
    motor2.setSpeed(vecY);
    motor1.run(FORWARD);
    motor2.run(FORWARD);
}

void reverse() {
    motor1.setSpeed(vecY);
    motor2.setSpeed(vecY);
    motor1.run(BACKWARD);
    motor2.run(BACKWARD);
}

void turn() {
    /*
    Basically, a turn is a reversal of one engine, and then turning that engine at X rate
    */


    // Turning starboard
    if (stickX >= 128) {
        motor1.setSpeed(vecY);
        motor2.setSpeed(vecX);
        motor1.run(FORWARD);
        motor2.run(BACKWARD);
    // Turning port
    } else {
        motor1.setSpeed(vecX);
        motor2.setSpeed(vecY);
        motor1.run(BACKWARD);
        motor2.run(FORWARD);
    }
}
