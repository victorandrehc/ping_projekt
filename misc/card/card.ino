#include <SPI.h>
#include <SD.h>

File root;
File getFile(File dir);

void setup(){
  Serial.begin(9600);
  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  root=SD.open("/");
  //printDirectory(root, 0);

  File myFile=getFile(root);
  
  while (myFile.available()) {
    Serial.print((char)myFile.read());
  }

}

void loop(){

}

File getFile(File dir){
  //returns object from the first file on the SD card
  File entry;
  entry=dir.openNextFile();
  if(!entry){
    Serial.println("Error");
    while(true);
  }else if(entry.isDirectory()){
    return getFile(entry);
  }else{
    Serial.println(entry.name());
    return SD.open(entry.name());
  }
}


