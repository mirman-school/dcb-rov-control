#include <AFMotor.h>
int stickX = 0; // motor 1 joystick
int stickY = 0; // motor 2 joystick
int rate1 = 0; // motor 1 speed
int rate2 = 0; // motor 2 speed
int rateMod1 = 0; // motor 1 speed modifier
int rateMod2 = 0; // motor 2 speed modifier


/*
Here we're going to create some helper variables to name the direction instructions.
*/
String AHEAD = "AHEAD";
String BACK = "BACK";
String PORT = "PORT";
String STARBOARD = "STARBOARD";
String STOP = "STOP";


/* 
Software definitions for both motors so we can control them
*/
AF_DCMotor motor1(1); 
AF_DCMotor motor2(2);

/*
setup() runs when the Arduino turns on. We put anstickY code needed to
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
    rate1 = map(stickX, 501, 1023, 0, 255);
    rate2 = map(stickY, 501, 1023, 0, 255);
    rateMod1 = map(stickX, 509, 1023, 0, 255);
    rateMod2 = map(stickY, 0, 509, 255, 0);

    // Turn these numbers into a simple direction command
    String direction = translate();

    /*
    When we have a set of known values for a variable and a long conditional, we can use a switch-case structure instead of if-else trees.
    */
    
    switch (direction) {
        case AHEAD:
            motor1.run(FORWARD);
            motor2.run(FORWARD);
            break;
        case BACK:
            motor1.run(BACKWARD);
            motor2.run(BACKWARD);
            break;
        case PORT:
            motor1.run(FORWARD);
            motor2.run(FORWARD);
            break;
        case STARBOAD:
            motor1.run(FORWARD);
            motor2.run(FORWARD);
            break;
        default:
            break;
    }


    // Print the data from the device
    Serial.print(stickX);
    Serial.print('-');
    Serial.println(stickY);
    Serial.print(rate1);
    Serial.print('/');
    Serial.print(rate2);
    Serial.print('_');
    Serial.print(rateMod1);
    Serial.print('_');
    Serial.println(rateMod2);
}

// Turn numbers into a direction command
String translate() {

    int speedX = constrain((rate1-rateMod1), 0, 255);
    int speedY = constrain((rate2+rateMod2), 0, 255);
    motor1.setSpeed(speedX);
    motor2.setSpeed(speedY);

    if(stickX < 501 || stickY < 509 || stickY > 509){
        return FORWARD;
    } else if (stickX > 501){
        return BACKWARD;
    } else {
        return STOP;
    }
}

int treatValue(int data) {
  return (data * 9 / 1024) + 48;
}


