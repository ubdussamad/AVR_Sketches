/*
  EMDSUBSYSTEMS
  AutomatonX1 - Automation Series Product Routine
  Author: ubdussamad <ubdussamad@gmail.com>
  Ref: 27 NOV 2019
  Coyright 2019 EMDSUBSYSTEMS


  We have GPIO -> 13 , 12 , 14 , 16 , 4 , 5 , 10 , 9
*/


#include <ESP8266WiFi.h>
#include <string>

#define TID 1000
#define MAX_RECONNECTION_WAIT_TIME 20 // Multiple of 0.5 seconds
#define GPIO_NUM 5

#ifndef STASSID
#define STASSID "Galaxy A50"
#define STAPSK  "zebrafamily"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

int GPIOS[8] = {14 , 12 , 13 , 4 , 5 }; //, 10 , 9}; 
void setup() {

  // We begin at 115200 Bits/Sec Data/Baud rate.
  Serial.begin(115200);

  
  //    We have GPIO -> 13 , 12 , 14 , 16 , 4 , 5 , 10 , 9
  

  // Loop over GPIO pin array and keep setting pinmodes
  for (int i=0;i<GPIO_NUM;i++) {
    pinMode( GPIOS[i] , OUTPUT);
    digitalWrite( GPIOS[i] , 0);
  }

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print(F("EMD SUBSYSTEMS"));
  Serial.print(F("\n-----------------------------------------"));
  Serial.print(F("\nHome Automaton Utility Prototype"));
  Serial.print(F("\nVersion 0.1"));
  Serial.print(F("\nRef: 27 Nov 18"));
  Serial.print(F("\nAuthor: Ubdussamad <ubdussamad@gmail.com>"));
  Serial.print(F("\nConnecting to Host AP named: "));
  Serial.println(ssid);
  Serial.print(F("\nPlease Wait"));

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("\nStation Successfully connected to AP."));

  // Start the server
  server.begin();
  Serial.println(F("\nServer Running .. @"));

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  
  if (WiFi.status() != WL_CONNECTED) {
      delay(2000);
      int time_delta = 0;
      Serial.print(F("\nRe-Connecting to Host AP named: "));
      Serial.println(ssid);
      Serial.print(F("\nPlease Wait"));
    
      //WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) {
        if (time_delta++ > MAX_RECONNECTION_WAIT_TIME ) { // About 10 secs
          Serial.print(F("\n\n||||||||||||||RESTARTING MODULE|||||||||||||||||\n\n"));
          ESP.restart();
          break;
        }
        delay(500);
        Serial.print(F("."));
      }
      Serial.println();
      Serial.println(F("\nStation Successfully connected to AP."));

  }
  
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println(F("NEW Client PING"));

  client.setTimeout(5000); // default is 1000

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(F("_____________________"));
  Serial.println(F("Request Type: "));
  Serial.println(req);
  // Match the request
  if (req.indexOf(F("/stat")) != -1) {
    Serial.println("\nEMD Subsystems - AUTOMATON-X1\nPID: AX1\nDevice UID:");
    Serial.println(TID);
    Serial.println("\n");
  }

  else if (req.indexOf(F("/config/update_credentials/")) != -1) {
    Serial.println("New Credentials Stored, the smart switch will try to connect\n"
                  "to this new WifiRouter (AP) first then if it fails then it'll\n"
                  "try to connect to the wifi with last credentials.= if the new one\n"
                  "fails to connect, so that you dont loose access if you make a mistake.\n"
                  "Then you can try updating the credentials again. If you fail to prvide\n"
                  "the correct credentials this time then you'll have to contact the serivce\n"
                  "provider who installed this system in your property.\n");
    Serial.println(req);
  }
  
  else if (req.indexOf(F("/gpio")) != -1) {
      Serial.println(F("\nGPIO Command"));
      for (int i=0; i<GPIO_NUM; i++) {
        if (req.indexOf( "/"+String(GPIOS[i])+"0" ) !=-1) {
          // Turn the line off
          Serial.println( " Setting GPIO: " + String(GPIOS[i]) + "LOW" );
          digitalWrite( GPIOS[i] , 0);
          
        }
        else if (req.indexOf( "/"+String(GPIOS[i])+"1" ) !=-1) {
          // Turn the line off
          Serial.println( " Setting GPIO: " + String(GPIOS[i]) + "HIGH" );
          digitalWrite( GPIOS[i] , 1);
        }
      }
  }
  
  else {
    Serial.println(F("Invalid Request: "));
    //Serial.println(F(req));
    for (int i=0;i<GPIO_NUM;i++){
      int parity = digitalRead(GPIOS[i]);
      digitalWrite(GPIOS[i], parity);
    }
  }

  // read/ignore the rest of the request
  // do not client.flush(): it is for output only, see below
  while (client.available()) {
    // byte by byte is not very efficient
    client.read();
  }

  // Send the response to the client
  // it is OK for multiple small client.print/write,
  // because nagle algorithm will group them into one single packet
  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
  ""
  "<!DOCTYPE HTML>\r\n<html>\r"
  "<head> <title> Automatonx1 - EMD Subsystems </title>"
  "<meta charset=\"utf-8\">"
  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
  "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/css/bootstrap.min.css\">"
  "<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js\"></script>"
  "<script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/js/bootstrap.min.js\"></script>"
  "</head>"
  "<body style=\"background: #F2F6DF;\">"
  "<br/>"
  "<h3 style=\"color:rgb(100,100,100);\" align=\"center\">EMD Subsystems</h3>"
  "<h4 style=\"color: #0D5EA5; \" align=\"center\">AutomatonX1</h4>"
  "<h5 style=\"color: #0D5EA5; \" align=\"center\">Automation Series Product Line</h5>"
  "<h5 align=\"center\">Author: ubdussamad <ubdussamad@gmail.com> </h5>"
  "<h5 align=\"center\">Ref: 27 NOV 2019 </h5>"
  "<div align=\"center\">"
  "<br/> <br/> <h5 align=\"center\"> Control Relay # </h4>"
  ));

  
  String IP = IpAddress2String(WiFi.localIP());
  for (int i=0;i<GPIO_NUM;i++) {
  char buff[10000];
  sprintf(buff,
  "<br/><a href='http://%s/gpio/%d1' class=\"btn btn-primary\" align=\"center\" role=\"button\" > ON-%d </a><a href='http://%s/gpio/%d0' class=\"btn btn-danger\" align=\"center\" role=\"button\" > OFF-%d </a>"
  , IP.c_str() , GPIOS[i] , GPIOS[i] , IP.c_str() , GPIOS[i] , GPIOS[i]);
  client.print(buff);
  
  }
  
  client.print(F(
  "</div>"
  "<br/><br/>"
  "<footer style=\"background: rgb(100,100,100);color:white;\">"
  "<h10 align=\"center\"> © Copyright 2019 EMD Subsystems </h10>"
  "</footer>"
  "</body>"
  "</html>"));

  // The client will actually be *flushed* then disconnected
  // when the function returns and 'client' object is destroyed (out-of-scope)
  // flush = ensure written data are received by the other side
  Serial.println(F("Disconnecting from client"));
}

String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ;
}
