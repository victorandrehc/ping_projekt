#include <SPI.h>
#include <RFID.h>

#include "tad_clock_system.h"

#include <inttypes.h>

//FSM GLOBAL VARIABLES/OBJECTS
const int change_state_button=45;
const unsigned long t_debounce=50;
const unsigned long one_hour=3600*1000;

unsigned long t_state=0,t_ntp=0;
typedef enum states_t {READING_EMPLOYEE,INTERMEDIARY_1,WRITING_NEW_EMPLOYEE,INTERMEDIARY_2};
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

void setup() {
  Serial.begin(9600);

  //FIFO INIT
  Employee employees_array[]={Employee("VICTOR",21425213114136),Employee("PAUL",2)};
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
    uint64_t v=0;
    for(int i=0;i<5;i++)
    {
    //Serial.print(RC522.serNum[i],DEC);
    //Serial.println(RC522.serNum[i],HEX); //to print card detail in Hexa Decimal format
      v=(v<<8)|(uint64_t)RC522.serNum[i];
    }
    Serial.print( (v) ); //TODO: insert mechanism to print a uint64_t
    Serial.println();
    Serial.println();
  }
  delay(1000);
}









