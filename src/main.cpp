#include <Arduino.h>

#include "FS.h" // File System
#include "SD.h" // SD Cards
#include "SPI.h" // Serial Peripheral Interface


TaskHandle_t Task1;
TaskHandle_t Task2;

void Task1code( void * pvParameters );
void Task2code( void * pvParameters );


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


  xTaskCreatePinnedToCore(
    Task1code, /* Task function. */
    "Task1",   /* name of task. */
    10000,     /* Stack size of task */
    NULL,      /* parameter of the task */
    1,         /* priority of the task */
    &Task1,    /* Task handle to keep track of created task */
    0);        /* pin task to core 0 */


  xTaskCreatePinnedToCore(
    Task2code, /* Task function. */
    "Task2",   /* name of task. */
    10000,     /* Stack size of task */
    NULL,      /* parameter of the task */
    1,         /* priority of the task */
    &Task2,    /* Task handle to keep track of created task */
    1);        /* pin task to core 1 */





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

void Task1code(void * pvParameters) {
  while (1) {  // Run indefinitely
    for (int i = 0; i < 10; i++) {
      Serial.println(i);
      String WriteStr = "Hello " + String(i) + "\n";
      writeFile(SD, "/hello.txt", WriteStr.c_str(), true);
      delay(1000);
    }
  }
}


void Task2code(void * pvParameters) {
  while (1) {  // Run indefinitely
    for (int j = 2; j < 14; j++) {
      Serial.println(j);
      String WriteStr = "Hello " + String(j) + "\n";
      writeFile(SD, "/hi.txt", WriteStr.c_str(), true);
      delay(1000);
    }
  }
}

