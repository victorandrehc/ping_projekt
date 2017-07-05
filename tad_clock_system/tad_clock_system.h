#ifndef TAD_CLOCK_SYSTEM_H
#define TAD_CLOCK_SYSTEM_H

#include <Arduino.h>
#include <inttypes.h>
#include <SD.h>
#define NAME_LEN 10
#define ID_LEN 5
#define NAME_SEPARATOR 0x3D
#define ID_SEPARATOR 0x0A



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
public:
	TimeCell* first;
	int len;
	TimeRow(){
		len=0;
		first=NULL;
	};
	~TimeRow(){};
	void insert(Time t);
	void print();
	int remove();
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
public:
	int len;
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
	bool timeToSync(){
		//return (current_time-last_sync_time>=3600 || millis()<t);//3600 Numbers of seconds in 60 minutes
		return false;//TODO: Remove this when actually updates time
	}
public:
	TimeHandler();
	~TimeHandler(){};
	void updateTime();
	void printDate();
	unsigned long getCurrentTime(){
		return current_time;
	}


	
};

#endif
