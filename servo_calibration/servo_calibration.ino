#include <Servo.h>

Servo S1;
Servo S2;

void setup() {
  // put your setup code here, to run once:
  
  S1.attach(36);
  S2.attach(38);

  S1.write(110);
  S2.write(75);
}

void loop() {
  // put your main code here, to run repeatedly:

}
