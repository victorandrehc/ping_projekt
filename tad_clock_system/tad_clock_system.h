#ifndef TAD_CLOCK_SYSTEM_H
#define TAD_CLOCK_SYSTEM_H

#include <Arduino.h>
#include <inttypes.h>
#include <SD.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

#define NAME_LEN 11
#define ID_LEN 5
#define NAME_SEPARATOR 0x3D
#define ID_SEPARATOR 0x0A
#define NTP_PACKET_SIZE 48
#define timeServer "pool.ntp.org"//"time.nist.gov" // time.nist.gov NTP server
#define UPLOAD_TIME 23
#define GMT 2L
#define FINISH_PACK "FINISH\n"


class Time{
	//class that manages Time, it has a timestamp, it contais all the relevant information of a TimeCell in a simple object
public:
	unsigned long timestamp;
	Time(){};
	~Time(){};
};

class TimeCell{
	//timeCell it is the cell from TimeRow FiFO
public:
	Time time;
	TimeCell* next;
	TimeCell(){};
	~TimeCell(){};
	void timeCpy(Time t);	
};

class TimeRow{
	//TimeRow is the head from Time FIFO, it points to the first item. Should be contained on an Employee object
private:
	int len;
public:
	TimeCell* first;
	TimeRow(){
		len=0;
		first=NULL;
	};
	~TimeRow(){};
	void insert(Time t);
	void print();
	unsigned long remove();
	int clear();
	int getLength(){
		return len;
	}
};

class Employee
{
	//it has the relevant information from an Employee, name ID and Time Fifo from this Employee
private:
public:
	char name[NAME_LEN];
	unsigned char id[ID_LEN];
	
	TimeRow timestamps;
	Employee(){};
	Employee(String name_,unsigned char* id_);
	Employee(char*name_,unsigned char* id_);
	~Employee(){//clear timestamp FIFO avoiding memory leak
		timestamps.clear();
	};
	int compareId(unsigned char * id_);
	void printId(unsigned char * id_);
	void printId();
	
};

class EmployeeCell{
	//same as TimeCell but to EmployeeRow
public:
	Employee employee;
	EmployeeCell* next;
	EmployeeCell(){};
	~EmployeeCell(){};
	void employeeCpy(Employee e);
	
};

class EmployeeRow{
	//EmployeeRow is the head from EMPLOYEE FIFO, it points to the first item. Should be contained a global object
private:
	int len;
public:
	EmployeeCell* first;
	EmployeeRow(){
		len=0;
		first=NULL;
	};
	~EmployeeRow(){};
	void insert(Employee e);
	void print();
	EmployeeCell* find(unsigned char* id,int* position); //find employee by id returns the cell
	EmployeeCell* find(char* name,int* position); //find employee by name returns the cell
	EmployeeCell* find(int position); //find employee by position returns the cell
	Employee* findEmployee(unsigned char* id,int* position); //find employee by id returns the object
	Employee* findEmployee(char *name,int* position); //find employee by name returns the object
	Employee* findEmployee(int position);//find employee by position returns the object
	int remove();
	int clear();
	int getLength(){
		return len;
	}
	
};

class TimeHandler{
	//make the Time Handling, the timestamp is a unsigned long variable and represents 
	//how many seconds passed after 01/01/1900 00:00:00
private:
	const unsigned long seventy_years = 2208988800UL; //represents 01/01/1970 00:00:00 was 
													  //introduced to gain space atthe variable
	unsigned long t;
	unsigned long last_sync_time;
	unsigned long current_time;
	byte packetBuffer[ NTP_PACKET_SIZE];
	EthernetUDP *udp;
	bool timeToSync(){ //return TRUE every Hour or when millis() overflows
		return (current_time-last_sync_time>=3600 || millis()<t);//3600 Numbers of seconds in 60 minutes
		//return true;//TODO: Remove this when actually updates time
	}
	void sendNTPpacket(); //send the NTP requistion to the Server
public:
	TimeHandler();
	~TimeHandler(){};
	bool updateTime();
	void printDate();
	void humanDate(unsigned long time,int* hours, int* minutes, int* seconds);
	void humanDate(int* hours, int* minutes, int* seconds);
	unsigned long getCurrentTime(){
		return current_time;
	}
	void setUdp(EthernetUDP *udp_);
	void getNTP();//sync the time
};

class UploadDataHandler{
	//Handles with the update data to google sheet throught a tellnet server.
	//this server is on server.py, his IP address and PORT must be passed from construtor
	//de Tellnet server can stays in other network
	//but must have be acessible from outside thought port fowarding in this case
private:
	EmployeeRow *employee_row;
	IPAddress server;
	unsigned int port;
	EthernetClient client; 
public:
	UploadDataHandler(IPAddress server_, unsigned int port_);
	~UploadDataHandler(){};
	void setEmployeeRow(EmployeeRow *e){
		employee_row=e;
	};
	void print();
	void upload();
	
};

#endif
