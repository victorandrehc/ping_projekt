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

unsigned long TimeRow::remove(){
	if(first!=NULL){
		TimeCell* removed_time=first;
		unsigned long result=removed_time->time.timestamp;
		first=removed_time->next;
		free(removed_time);
		len--;
		return result;
	}
	return 0;	
}
int TimeRow::clear(){
	while(remove());
	return 1;
}

Employee::Employee(String name_,unsigned char * id_){
	for(int i=0;i<ID_LEN;i++){
		id[i]=id_[i];
	}
	name_.toCharArray(name,NAME_LEN);
};

Employee::Employee(char*name_,unsigned char * id_){
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
		if(id[i]!=id_[i]){
			return 1;
		}
	}
	return 0;
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
	strcpy(employee.name,e.name);
	for(int i=0;i<ID_LEN;i++){
		employee.id[i]=e.id[i];
	}
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
		for (int i=0;i<5;i++){
			Serial.print(pivo->employee.id[i],HEX);
		}
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
EmployeeCell* EmployeeRow::find(int position){
	if(position>=len){
		return NULL;
	}
	if(position==0){
		return first;
	}else{
		EmployeeCell* pivo=first;
		for(int p=1; p<position;p++){				
				pivo=pivo->next;
		}
		if(pivo->next!=NULL){
			return pivo->next;
		}
	}
	return NULL;
}

Employee* EmployeeRow::findEmployee(unsigned char * id,int* position){
	EmployeeCell* emp=find(id,position);
	if(emp!=NULL){
		return &(emp->employee);
	}else{
		return NULL;
	}
}
Employee* EmployeeRow::findEmployee(char* name,int* position){
	EmployeeCell* emp=find(name,position);
	if(emp!=NULL){
		return &(emp->employee);
	}else{
		return NULL;
	}
}
Employee* EmployeeRow::findEmployee(int position){
	EmployeeCell* emp=find(position);
	if(emp!=NULL){
		return &(emp->employee);
	}else{
		return NULL;
	}
}
int EmployeeRow::remove(){
	if(first!=NULL){
		EmployeeCell* removed_employee=first;
		first=removed_employee->next;
		free(removed_employee);
		len--;
		return 1;
	}
	return 0;
}

int EmployeeRow::clear(){
	while(remove());
	return 1;
}

TimeHandler::TimeHandler(){
	t=millis();
	current_time=seventy_years+t/1000;
	//TODO: make current_time actualy syncs with the ntp 
	last_sync_time=current_time;
}
bool TimeHandler::updateTime(){
	uint8_t hours=0;
	if(timeToSync()==true ){
		//TODO: Sync time and remove code below
		getNTP();
		t=millis();
		unsigned long epoch=current_time-seventy_years;
		hours=(epoch  % 86400L) / 3600;
	}else{
		current_time=last_sync_time+(millis()-t)/1000;
		//Serial.print("TIME: ");
		//Serial.println(current_time-seventy_years);
		//Serial.println(millis());
	}
	return (hours==UPLOAD_TIME);
}

void TimeHandler::humanDate(unsigned long time,int* hours, int* minutes, int* seconds){
	unsigned long epoch=time-seventy_years;
	*hours=(epoch  % 86400L) / 3600;
	*minutes=(epoch  % 3600) / 60;
	*seconds=(epoch % 60);

}
void TimeHandler::humanDate(int* hours, int* minutes, int* seconds){
	humanDate(current_time,hours,minutes,seconds);
}

void TimeHandler::printDate(){
	

	int hours, minutes,seconds;
	humanDate(current_time,&hours,&minutes,&seconds);
	Serial.print(hours); // print the hour (86400 equals secs per day)
    Serial.print(':');
    if (minutes < 10) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.print(minutes); // print the minute (3600 equals secs per minute)
    Serial.print(':');
    if (seconds < 10) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.print(seconds); // print the second
  
}
void TimeHandler::setUdp(EthernetUDP *udp_){
	udp=udp_;
}

void TimeHandler::sendNTPpacket() {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp->beginPacket(timeServer, 123); //NTP requests are to port 123
  udp->write(packetBuffer, NTP_PACKET_SIZE);
  udp->endPacket();
}
void TimeHandler::getNTP(){
	sendNTPpacket();
	delay(1000);
	if (udp->parsePacket()) {
	    // We've received a packet, read the data from it
	    udp->read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

	    // the timestamp starts at byte 40 of the received packet and is four bytes,
	    // or two words, long. First, extract the two words:

	    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
	    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
	    // combine the four bytes (two words) into a long integer
	    // this is NTP time (seconds since Jan 1 1900):

	    current_time = (highWord << 16 | lowWord)+1;
	    last_sync_time=current_time;

	    Serial.println("GETTING DATE FROM NTP");
		printDate();
		Serial.println();    
  	}
  	Ethernet.maintain();

}

UploadDataHandler::UploadDataHandler(IPAddress server_, unsigned int port_){
	server=server_;
	port=port_;
}

void UploadDataHandler::print(){
	employee_row->print();
}

void UploadDataHandler::upload(){
	int employee_row_length=employee_row->getLength();

	for(int i=0;i<employee_row_length;i++){
		Employee* e=employee_row->findEmployee(i);
		if(e->timestamps.getLength()>=2){
			//TODO: SEND DATA TO CLOUD
			if(!client.connected() && client.connect(server,port)){
				Serial.println("connected");
			} else {
				// if you didn't get a connection to the server:
				Serial.println("connection failed");
			}
			if(client.connected()){
				while(e->timestamps.getLength()>=2){
					unsigned long timestamps[]={e->timestamps.remove(),e->timestamps.remove()};
					int hours_1,minutes_1,seconds_1;
					int hours_2,minutes_2,seconds_2;
					TimeHandler().humanDate(timestamps[0],&hours_1,&minutes_1,&seconds_1);
					TimeHandler().humanDate(timestamps[1],&hours_2,&minutes_2,&seconds_2);
					char msg[NAME_LEN+19];
					sprintf(msg,"%s;%02i:%02i:%02i;%02i:%02i:%02i\n",e->name,hours_1,minutes_1,seconds_1,hours_2,minutes_2,seconds_2);
					
					Serial.println(msg);
					client.write(msg);
				}
			}
		}
		
	}
	if(client.connected()){
		client.stop();
	}


}