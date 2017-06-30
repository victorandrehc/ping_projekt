#include <Arduino.h>
#include "tad_clock_system.h"


void TimeCell::timeCpy(Time t){
	time.timestamp=t.timestamp;
}

void TimeRow::insert(Time t){
	TimeCell* new_time;
	new_time=(TimeCell*)malloc(sizeof(TimeCell));
	new_time->timeCpy(t);
	new_time->next=NULL;

	if(first!=NULL){
		TimeCell* pivo=first;
			while(pivo->next!=NULL){				
				pivo=pivo->next;
			}
			pivo->next=new_time;
	}else{
		first=new_time;
	}
	len++;
}

void TimeRow::print(){
	TimeCell* pivo=first;
	Serial.print("TIMESTAMPS: [");
	while (pivo!=NULL){
		Serial.print(pivo->time.timestamp);
		if(pivo->next!=NULL){
			Serial.print(",");
		}
		pivo=pivo->next;
	}
	Serial.print("] length: ");
	Serial.print(len);
}

int TimeRow::remove(){
	if(first!=NULL){
		TimeCell* removed_time=first;
		first=removed_time->next;
		free(removed_time);
		len--;
		return 0;
	}
	return 1;	
}
int TimeRow::clear(){
	while(!remove());
	return 0;
}

Employee::Employee(String name_,int id_){
	//timestamps=NULL;
	id=id_;
	name_.toCharArray(name,NAME_LEN);
};

Employee::Employee(char*name_,int id_){
	//timestamps=NULL;
	id=id_;
	strcpy(name,name_);
};

void EmployeeCell::employeeCpy(Employee e){
	//employee.name=e.name;
	strcpy(employee.name,e.name);
	employee.id=e.id;
	employee.timestamps=e.timestamps;
}

void EmployeeRow::insert(Employee e){
	EmployeeCell* new_employee;
	new_employee=(EmployeeCell*)malloc(sizeof(EmployeeCell));
	new_employee->employeeCpy(e);
	new_employee->next=NULL;

	if(first!=NULL){
		EmployeeCell* pivo=first;
			while(pivo->next!=NULL){				
				pivo=pivo->next;
			}
			pivo->next=new_employee;
	}else{
		first=new_employee;
	}
	len++;
}

void EmployeeRow::print(){
	EmployeeCell* pivo=first;
	Serial.println("EMPLOYEES\n[");
	while (pivo!=NULL){
		Serial.print("    ");
		Serial.print(pivo->employee.name);
		Serial.print(" ");
		pivo->employee.timestamps.print();
		if(pivo->next!=NULL){
			Serial.println(",");
		}
		pivo=pivo->next;
	}
	Serial.print("\n] length: ");
	Serial.println(len);
}

EmployeeCell* EmployeeRow::find(int id,int* position){
	if(first!=NULL){
		if(first->employee.id==id){
			*position=0;
			return first;
		}else{
			EmployeeCell* pivo=first;
			for(*position=1; pivo->next!=NULL && pivo->next->employee.id!=id;*position=*position+1){				
				pivo=pivo->next;
			}
			if(pivo->next!=NULL){
				return pivo->next;
			}
		}		
	}
	*position=-1;
	return NULL;	
}

EmployeeCell* EmployeeRow::find(char* name,int* position){
	if(first!=NULL){
		if(!strcmp(first->employee.name,name)){
			*position=0;
			return first;
		}else{
			EmployeeCell* pivo=first;
			for(*position=1; pivo->next!=NULL && strcmp(pivo->next->employee.name,name);*position=*position+1){				
				pivo=pivo->next;
			}
			if(pivo->next!=NULL){
				return pivo->next;
			}
		}		
	}
	*position=-1;
	return NULL;	
}

Employee* EmployeeRow::findEmployee(int id,int* position){
	return &((this->find(id,position))->employee);
}
Employee* EmployeeRow::findEmployee(char* name,int* position){
	return &((this->find(name,position))->employee);
}

int EmployeeRow::remove(){
	if(first!=NULL){
		EmployeeCell* removed_employee=first;
		first=removed_employee->next;
		free(removed_employee);
		len--;
		return 0;
	}
	return 1;
}

int EmployeeRow::clear(){
	while(!remove());
	return 0;
}