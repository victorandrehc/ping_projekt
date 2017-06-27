#include "tad_clock_system.h"
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("\n");
  TimeStack stack;
  Time t1;
  t1.timestamp=millis();
  stack.insert(t1);
  stack.print();
  delay(500);
  Time t2;
  t2.timestamp=millis();
  stack.insert(t2);
  stack.print();
}

void loop() {
  // put your main code here, to run repeatedly:
}

