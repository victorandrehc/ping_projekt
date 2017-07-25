#include <SPI.h>
#include <RFID.h>
#include <SD.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

#include "tad_clock_system.h"

#include <inttypes.h>
//FSM GLOBAL VARIABLES/OBJECTS
const int change_state_button=45;
const int change_letter=42;
const int enter=44;

const unsigned long t_debounce=50;
const unsigned long one_second=1000;
uint8_t number_seconds=0,number_minutes=0;

unsigned long t_main_state=0;
enum main_states_t {READING_EMPLOYEE,INTERMEDIARY_1,WRITING_NEW_EMPLOYEE,INTERMEDIARY_2};
main_states_t main_state;

unsigned long t_new_employee_state=0;
enum new_employee_states_t {READING_ID_1,READING_ID_2,NEXT_LETTER,NEXT_POSITION};
new_employee_states_t new_employee_state;

char new_name[NAME_LEN];
int postion_new_name=0;
char letter='A';

//FIFO GLOBAL VARIABLES/OBJECTS
EmployeeRow employees_row;

//ETHERNET GLOBAL OBJECTS
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
unsigned int localPort = 8888;       // local port to listen for UDP packets
EthernetUDP Udp;

//TIME HANDLER GLOBAL OBJECTS
TimeHandler time;

//UploadDataHandler Object;
IPAddress server(192, 168, 2, 102);
unsigned int server_port=5555;
UploadDataHandler upload_data_handler(server,server_port);

//RFID GLOBAL VARIABLES/OBJECTS
const int SDA_DIO=5;
const int RESET_DIO=4;

//CREATE AN INSTANCE OF THE RFID LIBRARY
RFID RC522(SDA_DIO, RESET_DIO);

//CREATE AN FILE OBJECT
File getFile(File dir);
unsigned long deltat(unsigned long t);
void printState();
void readEmployee();
void reset_new_employee_state();

unsigned char v1[5]={0xD6,0xFC,0x83,0x8D,0x24},v2[5]={0xA6,0x8E,0x6B,0x90,0xD3};

void setup() {
  Serial.begin(9600);
  SPI.begin();
  //ETHERNET BEGIN
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for (;;)
      ;
  }
  Udp.begin(localPort);
  time.setUdp(&Udp); 
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
  upload_data_handler.setEmployeeRow(&employees_row);
  //FSM INIT
  main_state=READING_EMPLOYEE;
  pinMode(change_state_button, INPUT);
  t_main_state=millis();
  time.getNTP();
  Serial.println("SETUP FINISHED");
}

void loop() {
  switch (main_state) {
      case READING_EMPLOYEE:
        //TODO: READING EMPLOYEE
        readEmployee();
        if(digitalRead(change_state_button)==HIGH){
          main_state=INTERMEDIARY_1;
          t_main_state=millis();
        }
        break;
      case INTERMEDIARY_1:
        if(deltat(t_main_state)>=t_debounce && digitalRead(change_state_button)==LOW){
          main_state=WRITING_NEW_EMPLOYEE;
        }
        break;
      case WRITING_NEW_EMPLOYEE:
        //TODO: WRITING_EMPLOYEE
        printState();
        if(digitalRead(change_state_button)==HIGH){
          main_state=INTERMEDIARY_2;
          t_main_state=millis();
        }
        break;
      case INTERMEDIARY_2:
        if(deltat(t_main_state)>=t_debounce && digitalRead(change_state_button)==LOW){
          main_state=READING_EMPLOYEE;
        }
        break;
      default:
        main_state=READING_EMPLOYEE;
        Serial.println("RESETING FSM");
  }
  bool is_upload_time=time.updateTime();
  if(is_upload_time==true || true){
    upload_data_handler.upload();
  }
}

unsigned long deltat(unsigned long t){
  return(millis()-t);
}

void printState(){
  switch (main_state) {
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

