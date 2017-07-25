#include <SPI.h>
#include <RFID.h>
#include <SD.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <LiquidCrystal.h>

#include "tad_clock_system.h"

#include <inttypes.h>
//FSM GLOBAL VARIABLES/OBJECTS
const int change_state_button=45;
const int change_letter=42;
const int enter_button=44;

const unsigned long t_debounce=50;
const unsigned long one_second=1000;
uint8_t number_seconds=0,number_minutes=0;

unsigned long t_main_state=0;
enum main_states_t {READING_EMPLOYEE,INTERMEDIARY_1,WRITING_NEW_EMPLOYEE,INTERMEDIARY_2};
main_states_t main_state;

unsigned long t_new_employee_state=0;
enum new_employee_states_t {INIT,READING_ID_1,READING_ID_2,IDLE,NEXT_LETTER,ENTER};
new_employee_states_t new_employee_state;

char new_name[NAME_LEN];
int postion_new_name=0;
char letter=64;
unsigned char id_1[ID_LEN];
bool enter_pressed_once=false;

//LCD
LiquidCrystal lcd(22, 23, 30,31,32,33);

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
void write_new_employee();

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
  /*Employee employees_array[]={Employee("AABBCCDDEE",v2)};
  for(int i=0;i<1;i++){
    //TODO: make it read from card
    employees_row.insert(employees_array[i]);
  }*/
  employees_row.print();  
  upload_data_handler.setEmployeeRow(&employees_row);
  //LCD INIT
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("INITIALIZING");

  //FSM INIT
  main_state=READING_EMPLOYEE;
  pinMode(change_state_button, INPUT);
  pinMode(enter_button, INPUT);
  pinMode(change_letter, INPUT);
  lcd.clear();
  t_main_state=millis();
  time.getNTP();
  Serial.println("SETUP FINISHED");

}

void loop() {
  switch (main_state) { //FSM that controls the modes (reading hours data or recording a new employee)
      case READING_EMPLOYEE:
        readEmployee();
        if(digitalRead(change_state_button)==HIGH){
          main_state=INTERMEDIARY_1;
          t_main_state=millis();
        }
        break;
      case INTERMEDIARY_1: //buffer state to avoid bouncing
        if(deltat(t_main_state)>=t_debounce && digitalRead(change_state_button)==LOW){
          main_state=WRITING_NEW_EMPLOYEE;
          reset_new_employee_state();
          printState();
        }
        break;
      case WRITING_NEW_EMPLOYEE:
        write_new_employee();
        if(digitalRead(change_state_button)==HIGH){
          main_state=INTERMEDIARY_2;
          t_main_state=millis();
          reset_new_employee_state();
        }
        break;
      case INTERMEDIARY_2: //buffer state to avoid bouncing
        if(deltat(t_main_state)>=t_debounce && digitalRead(change_state_button)==LOW){
          main_state=READING_EMPLOYEE;
        }
        break;
      default:
        main_state=READING_EMPLOYEE;
        Serial.println("RESETING FSM");
  }
  bool is_upload_time=time.updateTime();
  if(is_upload_time==true){ //is_upload_time is true only if it is 23 hours and the NTP was just synced
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
  if (RC522.isCard()){ //If so then get its serial number 
    
    RC522.readCardSerial();
    Serial.println("Card detected:");
    for(int i=0;i<5;i++){
      Serial.print(RC522.serNum[i],HEX); //to print card detail in Hexa Decimal format
    }
    Serial.println();
    int p;
    Employee *emp=employees_row.findEmployee(RC522.serNum,&p);//search employee by ID
    lcd.clear();
    if(emp!=NULL){ // if employee exists in the FIFO Adds to it a timestamp
      Serial.println(emp->name);
      emp->printId();

      Time timestamp;
      timestamp.timestamp=time.getCurrentTime();
      emp->timestamps.insert(timestamp);
      Serial.println();
      employees_row.print();
      Serial.println();
      lcd.setCursor(0,0);
      lcd.print(emp->name);
      lcd.setCursor(0,1);
      char id[10];
      sprintf(id,"%02X%02X%02X%02X%02X",RC522.serNum[0],RC522.serNum[1],RC522.serNum[2],RC522.serNum[3],RC522.serNum[4]);
      lcd.print(id);
    }else{
      Serial.println("EMPLOYEE NOT FOUND");
      lcd.setCursor(0,0);
      lcd.print("EMPLOYEE MISSING");
      lcd.setCursor(0,1);
      char id[10];
      sprintf(id,"%02X%02X%02X%02X%02X",RC522.serNum[0],RC522.serNum[1],RC522.serNum[2],RC522.serNum[3],RC522.serNum[4]);
      lcd.print(id);
      unsigned long t_delay=millis();// wait a second so user has time to remove card from reader
      while(millis()-t_delay<=4000); 
    }    
    unsigned long t_delay=millis();// wait a second so user has time to remove card from reader
    while(millis()-t_delay<=1000);
    lcd.clear();
  }else{
    lcd.setCursor(0,0);
    lcd.print("INSERT CARD");
    lcd.setCursor(0,1);
    int hours,minutes,seconds;
    time.humanDate(&hours,&minutes,&seconds);
    char date[8];
    sprintf(date,"%02i:%02i:%02i",hours,minutes,seconds);
    lcd.print(date);
  }
}

void reset_new_employee_state(){
  //reset writng new employee fsm to original state
  new_employee_state=INIT;
  postion_new_name=0;
  memset(new_name,0,NAME_LEN);
  letter=64;
  memset(id_1,0,ID_LEN);
  enter_pressed_once=false;
}

void write_new_employee(){ 
  //Writing new employee FSM
  //it is designed to be inside another FSM
  // USer must first insert the card twice
  //only if both readings are right, user must insert a name
  //to insert a name user must select a letter using change_letter_button and press enter
  //is user presses enter twice in a row the name and Id are saved as am Employee object on FIFO
  switch (new_employee_state) {
      case INIT:
        Serial.println("\nINSERT YOUR CARD");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("INSERT YOUR CARD");
        new_employee_state=READING_ID_1;
        break;
      case READING_ID_1:
        if (RC522.isCard()){
          RC522.readCardSerial();
          Serial.println();
          for(int i=0;i<5;i++){
             Serial.print(RC522.serNum[i],HEX); //to print card detail in Hexa Decimal format
             id_1[i]=RC522.serNum[i];
          }
          unsigned long t_delay=millis();
          while(millis()-t_delay<=1000);
          Serial.println("\nINSERT YOUR CARD AGAIN");
          lcd.setCursor(0,0);
          lcd.print("INSERT YOUR CARD");
          lcd.setCursor(0,1);
          lcd.print("AGAIN");
          new_employee_state=READING_ID_2;
        }
        break;
      case READING_ID_2:        
        if (RC522.isCard()){
          RC522.readCardSerial();
          bool equal;
          Serial.println();
          for(int i=0;i<5;i++){
             Serial.print(RC522.serNum[i],HEX); //to print card detail in Hexa Decimal format
             equal=(id_1[i]==RC522.serNum[i]);
          }
          unsigned long t_delay=millis();
          while(millis()-t_delay<=1000);
          if(equal==true){
            new_employee_state=IDLE;
            Serial.println("\nCARDS MATCH\nUSE KEYBOARD TO INSERT A NAME");
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("CARDS MATCH");
            unsigned long t_delay=millis();
            while(millis()-t_delay<=1000);
            lcd.setCursor(0,0);
            lcd.print("USE KEYBOARD");
            lcd.setCursor(0,1);
            lcd.print("TO INSERT A NAME");
            lcd.setCursor(0,0);
          }else{
            new_employee_state=INIT;
            Serial.println("\nERROR: TRY AGAIN");
          }
        }
        break;
      case IDLE:
        if(digitalRead(change_letter)==HIGH){
          t_new_employee_state=millis();
          new_employee_state=NEXT_LETTER;
        }else if(digitalRead(enter_button)==HIGH){
          t_new_employee_state=millis();
          new_employee_state=ENTER;
        }
        break;
      case NEXT_LETTER:
        if(deltat(t_new_employee_state)>=t_debounce && digitalRead(change_letter)==LOW){
          if(letter!='Z'){
            letter++;
          }else{
            letter='A';
          }
        new_name[postion_new_name]=letter;
          enter_pressed_once=false;//reset flag that says if enter was just pressed
          Serial.println(new_name);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(new_name);
          new_employee_state=IDLE;
        }
        break;
      case ENTER:
        if(deltat(t_new_employee_state)>=(2*t_debounce) && digitalRead(enter_button)==LOW){
          if(enter_pressed_once==false){ // if it is first time in a row enter is pressed
            letter=64;
            postion_new_name=(postion_new_name+1)%(NAME_LEN-1);//NAME_LEN-1 because the last position must be \0
            enter_pressed_once=true;
          }else{ //if enter was pressed twice in a row
            Serial.println(new_name);
            for(int i=0;i<5;i++){
              Serial.print(id_1[i],HEX);
            }
            Serial.println();
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print(new_name);
            lcd.setCursor(0,1);
            lcd.print("RECORDED");
            unsigned long t_delay=millis();
            while(millis()-t_delay<=1000);
            lcd.clear();
            lcd.setCursor(0,0);
            Employee emp(new_name,id_1);
            employees_row.insert(emp);
            reset_new_employee_state();
            main_state=READING_EMPLOYEE;
          }
          new_employee_state=IDLE;  
        }
        break;
      default:
        reset_new_employee_state();
        break;
  }
}




