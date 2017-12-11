#include <AFMotor.h>
int x = 0; // motor 1 joystick
int y = 0; // motor 2 joystick
int rate1 = 0; // motor 1 speed
int rate2 = 0; // motor 2 speed
int rateMod1 = 0; // motor 1 speed modifier
int rateMod2 = 0; // motor 2 speed modifier

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);}
int treatValue(int data) {
  return (data * 9 / 1024) + 48;
}

void loop() {
  // put your main code here, to run repeatedly:
x = analogRead(A0);
y = analogRead(A1);
if(x < 501 || y < 509 || y > 509){
 rate1 = map(y, 501, 1023, 0, 255);
 rateMod1 = map(x, 509, 1023, 0, 255);
 rateMod2 = map(x, 0, 509, 255, 0);
 motor1.run(FORWARD);
 motor2.run(FORWARD);
 int rspeed1=constrain((rate1-rateMod1), 0, 255);
 int lspeed1=constrain((rate1+rateMod2), 0, 255);
  motor1.setSpeed(rspeed1);
  motor2.setSpeed(lspeed1);
}
else if (x > 501){
 rate2 = map(y, 501, 0, 0, 255);
 rateMod1 = map(x, 509, 1023, 0, 255);
 rateMod2 = map(x, 0, 509, 255, 0);
 motor1.run(BACKWARD);
 motor2.run(BACKWARD);
 int rspeed2 = constrain((rate2-rateMod1), 0, 255);
 int lspeed2 = constrain((rate2+rateMod2), 0, 255);
  motor1.setSpeed(rspeed2);
  motor2.setSpeed(lspeed2);
}
else{
  motor1.run(RELEASE);
  motor2.run(RELEASE);
}
Serial.print(x);
Serial.print('-');
Serial.println(y);
Serial.print(rate1);
Serial.print('/');
Serial.print(rate2);
Serial.print('_');
Serial.print(rateMod1);
Serial.print('_');
Serial.println(rateMod2);
}
