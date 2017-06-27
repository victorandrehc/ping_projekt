#include "tad_clock_system.h"
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("\n");
  TimeRow row;
  Time t1;
  t1.timestamp=millis();
  row.insert(t1);
  row.print();
  delay(500);
  Time t2;
  t2.timestamp=millis();
  row.insert(t2);
  row.print();
  row.clear();
  row.print();
}

void loop() {
  // put your main code here, to run repeatedly:
}