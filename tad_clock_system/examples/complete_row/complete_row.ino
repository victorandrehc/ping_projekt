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

  Serial.println("Inserting a timestamp on his timestamp row");
  Employee *emp=list.findEmployee(v1,&p);
  Time t;
  t.timestamp=millis();
  emp->timestamps.insert(t);


  Serial.print("Searching for the id which name is PAUL: ");
  (list.find(e2.name,&p))->employee.printId();
  Serial.print(". it is on postion ");
  Serial.println(p);
  
  Serial.println("Inserting a timestamp on his timestamp row");
  emp=list.findEmployee(e2.name,&p);
  t.timestamp=millis();
  emp->timestamps.insert(t);
  
  //emp.timestamps.print();
  Serial.println("Printing PAULS' TIMESTAMPS");
  (list.find(e2.name,&p))->employee.timestamps.print();
  Serial.println("Printing VICTOR's TIMESTAMPS");
  (list.find(e1.name,&p))->employee.timestamps.print();

  Serial.println("CLEANING EMPLOYEES");
  list.clear();
  list.print();


}

void loop() {
  // put your main code here, to run repeatedly:
}











