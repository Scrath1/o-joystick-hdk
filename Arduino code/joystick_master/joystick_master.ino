#include <Wire.h>
#include "btn_data_def.h"

#define SLAVE_ADDR 9
Btn_Meta_t msgBuffer = {0};
uint32_t dataReceived = 0;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("I2C Joystick Master Start");

}

void loop() {
  Serial.println("Starting loop");
  if(dataReceived == 0) {
    Wire.requestFrom(SLAVE_ADDR, sizeof(msgBuffer.rawData));
    Serial.println("Requested Message");
    }

  while(Wire.available() && dataReceived < sizeof(msgBuffer.rawData)){
    msgBuffer.rawData[dataReceived++] = Wire.read();
    }
  if(dataReceived==sizeof(msgBuffer.rawData)){
    Serial.println("data transfer complete");
    printBtn_Meta_t(msgBuffer);
    dataReceived=0;
    }
}
