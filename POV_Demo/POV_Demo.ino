// Persistance oF Vision Display
// Ref: 8 Mar 2020
// Author: Ubdussamad <ubdussamad2gmail.com>
#include <ESP8266WiFi.h>
char* ssid     = "Galaxy A50";
char* password = "zebrafamily";
WiFiServer server(80);
#define GPIO_NUM 5

int GPIOs [GPIO_NUM] = { 4, 10, 12, 13, 14 }; // , 11 , 6 , 8 , 7 , 12 };

void setup() {
  Serial.begin(115200);
  connectWiFi();
  // Some Greetings
  Serial.print("Goodey, I am from the Australian Government.");
  Serial.print("Initializing device....");

  // Initializing GPIO pins as output pins
//  for ( int i=0; i < GPIO_NUM; i++) {
//      pinMode( GPIOs[i] , OUTPUT);
//  }

  // Initializing one GPIO as INPUT for rotation detection.
  // Rotation speed is detected and timings are adjusted that way.
  //pinMode( 6, INPUT);

  Serial.print("Initialization Complete, Starting Mainloop.");
  
}


void loop() {
     // Routine for looping the Control.
     WiFiLocalWebPageCtrl(check_rpm());
     delay(3000);
     
}

unsigned long check_rpm ( void ) {
    // Routine for checking RPM of a wheel
    unsigned long delta;
    delta = 12;//pulseIn(11, HIGH);
    //Serial.println(duration);
    return( delta );
    }




void WiFiLocalWebPageCtrl(unsigned long dlt)
{
  WiFiClient client = server.available();   // listen for incoming clients
  //client = server.available();
  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print("Temperature now is: ");
            client.print(dlt);
            client.print("<br>");
            client.print("<br>");
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}



void connectWiFi(void)
{
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  server.begin();
}
