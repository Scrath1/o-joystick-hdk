#include <Wire.h>

#define SLAVE_ADDR 9

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("I2C Master Demonstration");

}

String answer="";
int lastMessageIndex=-1;
void loop() {
  Serial.println("Starting loop");
  Wire.requestFrom(SLAVE_ADDR, 64);
  Serial.println("Requested Message");
  String response = "";
  while(Wire.available()){
    char b = Wire.read();
    response +=b;
    }
  Serial.println(response);
  if(response.startsWith("[")){
    int messageNumber = response.charAt(1) - '0';
    if(messageNumber==0){
      answer="";
      lastMessageIndex=-1;
      } 
    String filteredRes = response.substring(3,response.indexOf('|'));
    answer=answer+filteredRes;
    Serial.println("Test");
    if(lastMessageIndex>messageNumber){ // Next transmission has started with new indexes
      Serial.println(answer);
      Serial.println("\nEND\n");
      }
    lastMessageIndex=messageNumber;
    }
}
