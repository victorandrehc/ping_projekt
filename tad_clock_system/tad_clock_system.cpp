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

Employee::Employee(String name_,unsigned char * id_){
	//timestamps=NULL;
	//id=id_;
	for(int i=0;i<ID_LEN;i++){
		id[i]=id_[i];
	}
	name_.toCharArray(name,NAME_LEN);
};

Employee::Employee(char*name_,unsigned char * id_){
	//timestamps=NULL;
	//id=id_;
	for(int i=0;i<ID_LEN;i++){
		id[i]=id_[i];
	}
	strcpy(name,name_);
};

int Employee::compareId(unsigned char * id_){
	int ret=0;
	for(int i=0;i<ID_LEN;i++){
		//Serial.println("-----");
		//Serial.println(id[i],HEX);
		//Serial.println(id_[i],HEX);
		ret+=id[i]-id_[i];
	}
	//Serial.print(ret);
	return ret;
}

void Employee::printId(unsigned char * id_){
	for(int i=0;i<ID_LEN;i++){
		Serial.print(id_[i],HEX);
	}
}
void Employee::printId(){
	for(int i=0;i<ID_LEN;i++){
		Serial.print(id[i],HEX);
	}
}
void EmployeeCell::employeeCpy(Employee e){
	//employee.name=e.name;
	strcpy(employee.name,e.name);
	for(int i=0;i<ID_LEN;i++){
		employee.id[i]=e.id[i];
	}
	//employee.id=e.id;
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

EmployeeCell* EmployeeRow::find(unsigned char * id,int* position){
	if(first!=NULL){
		if(first->employee.compareId(id)==0){
			*position=0;
			return first;
		}else{
			EmployeeCell* pivo=first;
			for(*position=1; pivo->next!=NULL && pivo->next->employee.compareId(id);*position=*position+1){				
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

Employee* EmployeeRow::findEmployee(unsigned char * id,int* position){
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

TimeHandler::TimeHandler(){
	t=millis();
	current_time=seventy_years+t/1000;
	//TODO: make current_time actualy syncs with the ntp 
	last_sync_time=current_time;
}
void TimeHandler::updateTime(){
	if(timeToSync()==true ){
		//TODO: Sync time and remove code below
		t=millis();
	}else{
		unsigned long new_t=millis();
		current_time+=(new_t-t)/1000;
		if(new_t-t>=1000){
			t=new_t;			
			Serial.print("TIME: ");
			Serial.println(current_time-seventy_years);
			Serial.println(new_t);
		}
	}
}

void TimeHandler::printDate(){
	

	unsigned long epoch=current_time-seventy_years;
	Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    Serial.print(':');
    if (((epoch % 3600) / 60) < 10) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
    Serial.print(':');
    if ((epoch % 60) < 10) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.print(epoch % 60); // print the second
  
}