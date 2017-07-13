#ifndef TAD_CLOCK_SYSTEM_H
#define TAD_CLOCK_SYSTEM_H

#include <Arduino.h>
#include <inttypes.h>
#include <SD.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

#define NAME_LEN 10
#define ID_LEN 5
#define NAME_SEPARATOR 0x3D
#define ID_SEPARATOR 0x0A
#define NTP_PACKET_SIZE 48
#define timeServer "pool.ntp.org"//"time.nist.gov" // time.nist.gov NTP server
#define UPLOAD_TIME 23


class Time{
public:
	unsigned long timestamp;
	Time(){};
	~Time(){};
};

class TimeCell{
public:
	Time time;
	TimeCell* next;
	TimeCell(){};
	~TimeCell(){};
	void timeCpy(Time t);	
};

class TimeRow{
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
};

class Employee
{
private:
public:
	char name[NAME_LEN];
	unsigned char id[ID_LEN];
	
	TimeRow timestamps;
	Employee(){};
	Employee(String name_,unsigned char* id_);
	Employee(char*name_,unsigned char* id_);
	~Employee(){
		timestamps.clear();
	};
	int compareId(unsigned char * id_);
	void printId(unsigned char * id_);
	void printId();
	
};

class EmployeeCell{
public:
	Employee employee;
	EmployeeCell* next;
	EmployeeCell(){};
	~EmployeeCell(){};
	void employeeCpy(Employee e);
	
};

class EmployeeRow{
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
	EmployeeCell* find(unsigned char* id,int* position);
	EmployeeCell* find(char* name,int* position);
	Employee* findEmployee(unsigned char* id,int* position);
	Employee* findEmployee(char *name,int* position);
	int remove();
	int clear();
	
};

class TimeHandler{
private:
	const unsigned long seventy_years = 2208988800UL;
	unsigned long t;
	unsigned long last_sync_time;
	unsigned long current_time;
	byte packetBuffer[ NTP_PACKET_SIZE];
	EthernetUDP *udp;
	bool timeToSync(){
		return (current_time-last_sync_time>=3600 || millis()<t);//3600 Numbers of seconds in 60 minutes
		//return true;//TODO: Remove this when actually updates time
	}
	void sendNTPpacket();
public:
	TimeHandler();
	~TimeHandler(){};
	bool updateTime();
	void printDate();
	unsigned long getCurrentTime(){
		return current_time;
	}
	void setUdp(EthernetUDP *udp_);
	void getNTP();	
};

class UploadDataHandler{
private:
	EmployeeRow *employee_row;
public:
	UploadDataHandler(){};
	~UploadDataHandler(){};
	void setEmployeeRow(EmployeeRow *e){
		employee_row=e;
	};
	void print();
	void upload();
	
};

#endif
