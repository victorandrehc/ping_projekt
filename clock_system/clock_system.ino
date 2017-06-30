#include "tad_clock_system.h"

const int change_state_button=45;
const unsigned long t_debounce=50;
const unsigned long one_hour=3600*1000;
unsigned long t_state=0,t_ntp=0;

EmployeeRow employees_row;
typedef enum states_t {READING_EMPLOYEE,INTERMEDIARY_1,WRITING_NEW_EMPLOYEE,INTERMEDIARY_2};
states_t state=READING_EMPLOYEE;

unsigned long deltat();
void resett();
void printState();

void setup() {
  Serial.begin(9600);
  Employee employees_array[]={Employee("VICTOR",1),Employee("PAUL",2)};
  for(int i=0;i<2;i++){
    //TODO: make it read from card
    employees_row.insert(employees_array[i]);
  }
  employees_row.print();
  pinMode(change_state_button, INPUT);
  t_state=millis();
  t_ntp=millis();
}

void loop() {
  switch (state) {
      case READING_EMPLOYEE:
        //TODO: READING EMPLOYEE
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
  return(millis()-t);
}
void resett(){
  t=millis();
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