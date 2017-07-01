#ifndef TAD_CLOCK_SYSTEM_H
#define TAD_CLOCK_SYSTEM_H

#include <Arduino.h>
#include <inttypes.h>
#define NAME_LEN 10
#define ID_LEN 5



class Time{
public:
	long timestamp;
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

#endif
