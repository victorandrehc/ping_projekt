#include <SPI.h>
#include <RFID.h>

#include "tad_clock_system.h"

#include <inttypes.h>

//FSM GLOBAL VARIABLES/OBJECTS
const int change_state_button=45;
const unsigned long t_debounce=50;
const unsigned long one_hour=3600*1000;

unsigned long t_state=0,t_ntp=0;
enum states_t {READING_EMPLOYEE,INTERMEDIARY_1,WRITING_NEW_EMPLOYEE,INTERMEDIARY_2};
states_t state;

//FIFO GLOBAL VARIABLES/OBJECTS
EmployeeRow employees_row;

//RFID GLOBAL VARIABLES/OBJECTS
const int SDA_DIO=9;
const int RESET_DIO=8;

//CREATE AN INSTANCE OF THE RFID LIBRARY
RFID RC522(SDA_DIO, RESET_DIO);

unsigned long deltat();
void resett();
void printState();

void readEmployee();

unsigned char v1[5]={0xD6,0xFC,0x83,0x8D,0x24},v2[5]={0xD6,0xFC,0x83,0x8D,0x23};

void setup() {
  Serial.begin(9600);

  //FIFO INIT
  Employee employees_array[]={Employee("VICTOR",v1),Employee("PAUL",v2)};
  for(int i=0;i<2;i++){
    //TODO: make it read from card
    employees_row.insert(employees_array[i]);
  }
  employees_row.print();

  //RFID INIT
  SPI.begin(); 
  RC522.init();
  
  //FSM INIT
  state=READING_EMPLOYEE;
  pinMode(change_state_button, INPUT);
  t_state=millis();
  t_ntp=millis();
}

void loop() {
  switch (state) {
      case READING_EMPLOYEE:
        //TODO: READING EMPLOYEE
        readEmployee();
        if(digitalRead(change_state_button)==HIGH){
          state=INTERMEDIARY_1;
          resett();
        }
        break;
      case INTERMEDIARY_1:
        if(deltat()>=t_debounce && digitalRead(change_state_button==LOW)){
          state=WRITING_NEW_EMPLOYEE;
        }
        break;
      case WRITING_NEW_EMPLOYEE:
        //TODO: WRITING_EMPLOYEE
        if(digitalRead(change_state_button)==HIGH){
          state=INTERMEDIARY_2;
          resett();
        }
        break;
      case INTERMEDIARY_2:
        if(deltat()>=t_debounce && digitalRead(change_state_button==LOW)){
          state=READING_EMPLOYEE;
        }
        break;
      default:
        state=READING_EMPLOYEE;
        Serial.println("RESETING FSM");
  }
  if(millis()-t_ntp>=one_hour){
    t_ntp=millis();
    //TODO: ASK NTP FOR THE HOUR
    //IF IT IS FROM 23 TO 24 send to Google sheets
  }

}

unsigned long deltat(){
  return(millis()-t_state);
}
void resett(){
  t_state=millis(); 
}
void printState(){
  switch (state) {
      case READING_EMPLOYEE:
        Serial.print("READING_EMPLOYEE");  
        break;
      case INTERMEDIARY_1:
        Serial.print("INTERMEDIARY_1");
        break;
      case WRITING_NEW_EMPLOYEE:
        Serial.print("WRITING_NEW_EMPLOYEE");
        break;
      case INTERMEDIARY_2:
        Serial.print("INTERMEDIARY_2");
        break;
      default:
        Serial.print("UNKONOWN");
  }
}

void readEmployee(){
  if (RC522.isCard()){
    /* If so then get its serial number */
    RC522.readCardSerial();
    Serial.println("Card detected:");
    for(int i=0;i<5;i++){
      Serial.print(RC522.serNum[i],HEX); //to print card detail in Hexa Decimal format
    }
    Serial.println();
    int p;
    Serial.println((employees_row.find(RC522.serNum,&p))->employee.name);
    (employees_row.find(RC522.serNum,NULL))->employee.printId();
    Serial.println();
    Serial.println();
  }
  delay(1000);
}





