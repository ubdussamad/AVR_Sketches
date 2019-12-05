/*
 * ESP8266 SPIFFS Basic Reading and Writing File 
 *
 */

#include <ESP8266WiFi.h>
#include <FS.h>   //Include File System Headers

const char* filename = "/samplefile.txt";

  
void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();

  //Initialize File System
  if(SPIFFS.begin())
  {
    Serial.println("\nSPIFFS Initialize....ok");
  }
  else
  {
    Serial.println("\nSPIFFS Initialization...failed");
  }

//  //Format File System
//  if(SPIFFS.format())
//  {
//    Serial.println("\nFile System Formated");
//  }
//  else
//  {
//    Serial.println("\nFile System Formatting Error");
//  }

  //Create New File And Write Data to It
  //w=Write Open file for writing
//  File f = SPIFFS.open(filename, "w");
//  
//  if (!f) {
//    Serial.println("\nfile open failed");
//  }
//  else
//  {
//      //Write data to file
//      Serial.println("\nWriting Data to File");
//      f.print("This is sample data which is written in file");
//      f.close();  //Close file
//  }

}

void loop() {
  int i;
  
  //Read File data
  File f = SPIFFS.open(filename, "r");
  
  if (!f) {
    Serial.println("\nfile open failed");
  }
  else
  {
      Serial.println("\nReading Data from File:");
      //Data from file
      for(i=0;i<f.size();i++) //Read upto complete file size
      {
        Serial.print((char)f.read());
      }
      f.close();  //Close file
      Serial.println("\nFile Closed");
  }
  delay(5000);
}
