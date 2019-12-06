#include <FS.h>
 
void setup() {
 
  Serial.begin(115200);
  Serial.println();
 
  bool success = SPIFFS.begin();
 
  if (!success) {
    Serial.println("Error mounting the file system");
    return;
  }
 
 
  File file2 = SPIFFS.open("/ack.html", "r");
 
  if (!file2) {
    Serial.println("Failed to open file for reading");
    return;
  }
 
  Serial.println("File Content:");
 
  while (file2.available()) {
 
    Serial.write(file2.read());
  }
 
  file2.close();

  Dir dir = SPIFFS.openDir("/data");
  while (dir.next()) {
    Serial.print(dir.fileName());
    File f = dir.openFile("r");
    Serial.println(f.size());
}

 
}
void loop() {}
