#include <Arduino.h>

#include "FS.h" // File System
#include "SD.h" // SD Cards
#include "SPI.h" // Serial Peripheral Interface

void writeFile(fs::FS &fs, const char * path, const char * message, bool append = false){
  Serial.printf("Writing file: %s\n", path);

  File file;
  if (append) {
    file = fs.open(path, FILE_APPEND); // Open file in append mode
  } else {
    file = fs.open(path, FILE_WRITE); // Open file in write mode
  }

  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }

  if(file.print(message)){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void setup(){
  Serial.begin(115200);
  if(!SD.begin(5)){ // CS pin - 5 (SPI communication)
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

}

void loop(){
for (int i = 0; i < 10; i++) {
  Serial.println(i);
  String WriteStr = "Hello" + String(i) + "\n"; 
  writeFile(SD, "/hello.txt", WriteStr.c_str(), true);
  delay(1000);
}
}

