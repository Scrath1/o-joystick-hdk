#include <Wire.h>
#include "btn_data_def.h"

// Define Slave I2C Address
#define SLAVE_ADDR 9
// Define pins used for buttons
#define thumbstick1 A0
#define thumbstick2 A1
#define topHat1 A2
#define topHat2 A3
#define thumbHat1 8
#define thumbHat2 9
#define triggerButtons A7

#define brakeButton 16
#define topSideButton 15
#define pinkyButton 14

Btn_Meta_t msgBuffer = {0};

void setup() {
  Wire.begin(SLAVE_ADDR);
  Wire.onRequest(requestEvent);
  
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Starting Slave");
  pinMode(brakeButton, INPUT_PULLUP);
  pinMode(topSideButton, INPUT_PULLUP);
  pinMode(pinkyButton, INPUT_PULLUP);
}

void loop() {
  msgBuffer.data.thumbstick1_val=analogRead(thumbstick1);
  msgBuffer.data.thumbstick2_val=analogRead(thumbstick2);
  msgBuffer.data.topHat1_val=analogRead(topHat1);
  msgBuffer.data.topHat2_val=analogRead(topHat2);
  msgBuffer.data.thumbHat1_val=analogRead(thumbHat1);
  msgBuffer.data.thumbHat2_val=analogRead(thumbHat2);
  msgBuffer.data.triggerButtons_val=analogRead(triggerButtons);
  
  msgBuffer.data.brakeButton_val=digitalRead(brakeButton);
  msgBuffer.data.topSideButton_val=digitalRead(topSideButton);
  msgBuffer.data.pinkyButton_val=digitalRead(pinkyButton);
  
//  Serial.println("TS1:" + msgBuffer.data.thumbstick1_val);
//  Serial.println("TS2:" + msgBuffer.data.thumbstick2_val);
//  Serial.println("topH1:" + msgBuffer.data.topHat1_val);
//  Serial.println("topH2:" + msgBuffer.data.topHat2_val);
//  Serial.println("thumbH1:" + msgBuffer.data.thumbHat1_val);
//  Serial.println("thumbH2:" + msgBuffer.data.thumbHat2_val);
//  Serial.println("trigger:" + msgBuffer.data.triggerButtons_val);
//  Serial.println("brake:" + msgBuffer.data.brakeButton_val);
//  Serial.println("topSide:" + msgBuffer.data.topSideButton_val);
//  Serial.println("pinky:" + msgBuffer.data.pinkyButton_val);
}

void requestEvent(){
  Serial.println("Received Request Event");
  Wire.write(msgBuffer.rawData,sizeof(msgBuffer.rawData));
}
