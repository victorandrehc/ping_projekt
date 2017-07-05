#include <SPI.h>
#include <RFID.h>
#include <SD.h>

#include "tad_clock_system.h"

#include <inttypes.h>

//FSM GLOBAL VARIABLES/OBJECTS
const int change_state_button=45;
const unsigned long t_debounce=50;
const unsigned long one_second=1000;
uint8_t number_seconds=0,number_minutes=0;

unsigned long t_state=0;
enum states_t {READING_EMPLOYEE,INTERMEDIARY_1,WRITING_NEW_EMPLOYEE,INTERMEDIARY_2};
states_t state;

//FIFO GLOBAL VARIABLES/OBJECTS
EmployeeRow employees_row;

//TIME HANDLER GLOBAL OBJECTS
TimeHandler time;

//RFID GLOBAL VARIABLES/OBJECTS
const int SDA_DIO=9;
const int RESET_DIO=8;

//CREATE AN INSTANCE OF THE RFID LIBRARY
RFID RC522(SDA_DIO, RESET_DIO);

//CREATE AN FILE OBJECT
File getFile(File dir);
unsigned long deltat();
void resett();
void printState();

void readEmployee();

unsigned char v1[5]={0xD6,0xFC,0x83,0x8D,0x24},v2[5]={0xD6,0xFC,0x83,0x8D,0x23};

void setup() {
  Serial.begin(9600);
  SPI.begin(); 
  //RFID INIT
  RC522.init();

  //FIFO INIT
  Employee employees_array[]={Employee("VICTOR",v1),Employee("PAUL",v2)};
  for(int i=0;i<2;i++){
    //TODO: make it read from card
    employees_row.insert(employees_array[i]);
  }
  //employees_row.populate(myFile);
  employees_row.print();  
  
  //FSM INIT
  state=READING_EMPLOYEE;
  pinMode(change_state_button, INPUT);
  t_state=millis();
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
  time.updateTime();

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

File getFile(File dir){
  //returns object from the first file on the SD card
  File entry;
  entry=dir.openNextFile();
  if(!entry){
    Serial.println("Error");
    while(true);
  }else if(entry.isDirectory()){
    return getFile(entry);
  }else{
    Serial.println(entry.name());
    return SD.open(entry.name());
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
    Employee *emp=employees_row.findEmployee(RC522.serNum,&p);
    if(emp!=NULL){
      Serial.println(emp->name);
      emp->printId();

      Time timestamp;
      timestamp.timestamp=time.getCurrentTime();
      emp->timestamps.insert(timestamp);
      
      Serial.println();
    }else{
      Serial.println("EMPLOYEE NOT FOUND");
    }    
    unsigned long t_delay=millis();
    while(millis()-t_delay<=1000);
  }
}