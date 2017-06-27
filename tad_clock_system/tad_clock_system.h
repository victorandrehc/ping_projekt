#ifndef TAD_CLOCK_SYSTEM_H
#define TAD_CLOCK_SYSTEM_H

#include <Arduino.h>



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
	char name[10];
	int id;
	TimeRow timestamps;
	Employee(){};
	~Employee(){
		timestamps.clear();
	};
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
	EmployeeCell* find(int id,int* position);
	EmployeeCell* find(char* name,int* position);
	Employee* findEmployee(int id,int* position);
	Employee* findEmployee(char *name,int* position);
	int remove();
	int clear();
	
};
#endif
