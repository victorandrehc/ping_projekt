#include "tad_clock_system.h"
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("\n");

  EmployeeRow list;
  Employee e1;
  strcpy(e1.name,"VICTOR");
  e1.id=1;
  list.insert(e1);
  list.print();
  Employee e2;
  strcpy(e2.name,"PAUL");
  e2.id=2;
  list.insert(e2);
  list.print();
  int p;
  Serial.print("Searching for the name which id is 1: ");
  Serial.print((list.find(1,&p))->employee.name);
  Serial.print(". it is on postion ");
  Serial.println(p);

  Serial.println("Inserting a timestamp on his timestamp row");
  Employee *emp=list.findEmployee(1,&p);
  Time t;
  t.timestamp=millis();
  emp->timestamps.insert(t);


  Serial.print("Searching for the id which name is PAUL: ");
  Serial.print((list.find(e2.name,&p))->employee.id);
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











