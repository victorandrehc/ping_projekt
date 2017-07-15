/*
     TEMPERATURE_LOGGER_BY_USING_ARDUINO UNO AND ETHERNET SHIELD(HR911105A) 
      Here We Connect One Temperature Sensor To our Arduino UNO Board And Maintaining 
      A Log File Of Date And Temperature .And We Are Using  Google Spreadsheet To Store
      Our Data. For Storing Our Data In A Google Sheet We Need A Ethernet Shield To Connect 
      Our Arduino Board To Internet.
      CONNECCTION
      * LM35  Pin 1 Connected to Vcc Of Arduino UNO
      * LM35  Pin2 Connected to A0 Of Arduino Uno
      * LM35  Pin3 connected to GND Of Arduino UNO 
      * Mount Your Ethernet Shield On Your Board
      * Connect a Straight RJ45 Cable to your Ethernet Shield (HR911105A) From Your Router
 created 16 NOV 2016
 by SOUMYA RANJAN PANDA
 For any help contact info@deligence.com
 
 */



#include <SPI.h>
#include <Ethernet.h>

//-------------------------------------------------------------------------------
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //Setting MAC Address
char server[] = "api.pushingbox.com"; //pushingbox API server
IPAddress ip(192,168,1,2); //Arduino IP address. Only used when DHCP is turned off.
EthernetClient client; //define 'client' as object
String data; //GET query with data
float suhu; //suhu (bahasa Indonesia) means temperature
boolean koneksi = false;
//------------------------------------------------------------------------------
void setup() {
  
  Serial.begin(9600);

  if (Ethernet.begin(mac) == 0) {
  Serial.println("Failed to configure Ethernet using DHCP");
  Ethernet.begin(mac, ip);
  }

  delay(1000);
}
//------------------------------------------------------------------------------
void loop(){
  int baca_ADC = analogRead(A0); //read analog input on pin A7
   suhu = baca_ADC ; // ADC to celcius conversion
   kemasData(); //packing GET query with data
   Serial.println("connecting...");
   if (client.connect(server, 80)) {
     sendData();  
     koneksi = true; //connected = true
   }
   else{
     Serial.println("connection failed");
   }
  // loop
  while(koneksi){
    if (client.available()) {
    char c = client.read(); //save http header to c
    Serial.print(c); //print http header to serial monitor
    }
    if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
          Serial.print("Temperature Sent :");
          Serial.println(suhu); //print sent value to serial monitor
    client.stop(); 
          koneksi = false; 
          data = ""; //data reset
    }
  }
  delay(5000); // interval
  while(1);
}

void kemasData(){
  data+="";
  data+="GET /pushingbox?devid=v61E77FE5C368894"; //GET request query to pushingbox API
  data+="&name=";
  data+="'VICTOR'";
  data+="&in=";
  data+="'08:00:00'";
  data+="&out=";
  data+"'12:00:00'";
  data+="HTTP/1.1";
  Serial.println(data);
}
void sendData(){
  Serial.println("connected");
  client.println(data);
  client.println("Host: api.pushingbox.com");
  client.println("Connection: close");
  client.println();
}



