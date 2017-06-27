#include <Arduino.h>
#include "tad_clock_system.h"

void EmployeeCell::employeeCpy(Employee e){
	//employee.name=e.name;
	strcpy(employee.name,e.name);
	employee.id=e.id;
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
	Serial.print("[");
	while (pivo!=NULL){
		Serial.print(pivo->employee.name);
		if(pivo->next!=NULL){
			Serial.print(",");
		}
		pivo=pivo->next;
	}
	Serial.print("] length: ");
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
