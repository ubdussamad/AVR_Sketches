/*
  EMDSUBSYSTEMS
  AutomatonX1 - Automation Series Product Routine
  Author: ubdussamad <ubdussamad@gmail.com>
  Ref: 27 NOV 2019
  Coyright 2019 EMDSUBSYSTEMS


  We have GPIO -> 13 , 12 , 14 , 16 , 4 , 5 , 10 , 9
*/


#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "Galaxy A50"
#define STAPSK  "zebrafamily"
#endif
#define GPIO_NUM 3
const char* ssid = STASSID;
const char* password = STAPSK;

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

int GPIOS[8] = {13 , 4 , 5 }; //, 10 , 9}; 
void setup() {

  // We begin at 115200 Bits/Sec Data/Baud rate.
  Serial.begin(115200);

  
  //    We have GPIO -> 13 , 12 , 14 , 16 , 4 , 5 , 10 , 9
  

  // Loop over GPIO pin array and keep setting pimodes
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
  if (req.indexOf(F("/hi")) != -1) {
    Serial.println(F("\nHello there, have a good day! :)"));  
  }
  
  else if (req.indexOf(F("/gpio")) != -1) {
      //    We have GPIO -> 13 , 12 , 14 , 16 , 4 , 5 , 10 , 9
      Serial.println(F("\nGPIO Command"));
      for (int i=0; i<GPIO_NUM; i++) {
        if (req.indexOf( "/"+String(GPIOS[i])+"0" ) !=-1) {
          // Turn the line off
          Serial.println( " Setting GPIO: " + String(GPIOS[i]) + "LOW" );
          //Serial.println(String(GPIOS[i]));
          //Serial.println(F(" LOW. \n"));
          digitalWrite( GPIOS[i] , 0);
          
        }
        else if (req.indexOf( "/"+String(GPIOS[i])+"1" ) !=-1) {
          // Turn the line off
          Serial.println( " Setting GPIO: " + String(GPIOS[i]) + "HIGH" );
          //Serial.println(String(GPIOS[i]));
          //Serial.println(F(" HIGH. \n"));
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
  "<head> <title> EMD SUBSYSTEMS </title>"
  "<meta charset=\"utf-8\">"
  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
  "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/css/bootstrap.min.css\">"
  "<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js\"></script>"
  "<script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/js/bootstrap.min.js\"></script>"
  "</head>"
  "<body style=\"background: #F2F6DF;\">"
  "<br/>"
  "<h4 style=\"color:rgb(100,100,100);\" align=\"center\">EMDSUBSYSTEMS</h4>"
  "<h6 style=\"color: #0D5EA5; \" align=\"center\">AutomatonX1 - Automation Series Product Routine </h6>"
  "<h10 align=\"center\">Author: ubdussamad <ubdussamad@gmail.com> </h10>"
  "<h10 align=\"center\">Ref: 27 NOV 2019 </h10>"
  "<h10 align=\"center\">Coyright 2019 EMDSUBSYSTEMS </h10>"
  ));
  client.print(F(
  "<br/> <br/> <h8 align='center'> Control Relay # </h6>"
  "<br/><a href='http://"));
  client.print(WiFi.localIP());
  client.print(F(
  "/gpio/41' class=\"btn btn-primary\" role=\"button\" > ON-4 </a> "
  "<a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/40' class=\"btn btn-danger\" role=\"button\" > OFF-4 </a>"
  "</body>"
  "</html>"));

  // The client will actually be *flushed* then disconnected
  // when the function returns and 'client' object is destroyed (out-of-scope)
  // flush = ensure written data are received by the other side
  Serial.println(F("Disconnecting from client"));
}
