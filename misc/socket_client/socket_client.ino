#include <SPI.h>
#include <Ethernet.h>
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
unsigned int server_port=5555;
IPAddress server(192, 168, 2, 102);

EthernetClient client;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();
  //ETHERNET BEGIN
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for (;;)
      ;
  }
  if (client.connect(server, server_port)) {
    Serial.println("connected");
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  client.write("VICTOR;08:00:00;12:00:00");
  while(1);
}
