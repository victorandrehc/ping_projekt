#include "tad_clock_system.h"
unsigned char v1[5]={0xD6,0xFC,0x83,0x8D,0x24},v2[5]={0xD6,0xFC,0x83,0x8D,0x23};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("\n");
  EmployeeRow list;
  Employee e1("VICTOR",v1);
  list.insert(e1);
  list.print();
  Employee e2("PAUL",v2);
  list.insert(e2);
  list.print();
  int p;
  Serial.print("Searching for the name which id is DCF6838D24: ");
  Serial.print((list.find(v1,&p))->employee.name);
  Serial.print(". it is on postion ");
  Serial.println(p);

  Serial.print("Searching for the id which name is PAUL: ");
  (list.find(e2.name,&p))->employee.printId();
  Serial.print(". it is on postion ");
  Serial.println(p);
  list.clear();
  list.print();
}

void loop() {
  // put your main code here, to run repeatedly:
}
