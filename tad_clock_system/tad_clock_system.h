#ifndef TAD_CLOCK_SYSTEM_H
#define TAD_CLOCK_SYSTEM_H

#include <Arduino.h>

class Employee
{
private:
public:
	char name[10];
	int id;
	Employee(){};
	~Employee(){};
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
	int remove();
	int clear();
	
};

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

class TimeStack{
public:
	TimeCell* first;
	int len;
	TimeStack(){
		len=0;
		first=NULL;
	};
	~TimeStack(){};
	void insert(Time t);
	void print();
	int remove();
	int clear(){};
};

#endif
