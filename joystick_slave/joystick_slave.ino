#include <Wire.h>

// Define Slave I2C Address
#define SLAVE_ADDR 9
// Define Slave answer size
#define ANSWERSIZE 128
// Define how many byte/char of the readout string are transmitted in one package.
// Values higher than 27 cause the string to not be detected from the master for some reason
#define PACKAGE_MESSAGE_LENGTH 27
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

int thumbstick1_val;
int thumbstick2_val;
int topHat1_val;
int topHat2_val;
int thumbHat1_val;
int thumbHat2_val;
int triggerButtons_val;
int brakeButton_val;
int topSideButton_val;
int pinkyButton_val;

String readoutString="";
int requestCounter=0;
int requestCounterLimit=-1; // -1 = not initialized


void setup() {
  Wire.begin(SLAVE_ADDR);
  Wire.onRequest(requestEvent);
  
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(brakeButton, INPUT_PULLUP);
  pinMode(topSideButton, INPUT_PULLUP);
  pinMode(pinkyButton, INPUT_PULLUP);

  requestCounterLimit=(double)ANSWERSIZE/PACKAGE_MESSAGE_LENGTH+0.5; // determines maximum amount of requested packes before resetting the requestCounter
  Serial.println("requestCounterLimit has been set to: " + String(requestCounterLimit));
  //delay(10);
}

void loop() {
  thumbstick1_val=analogRead(thumbstick1);
  thumbstick2_val=analogRead(thumbstick2);
  topHat1_val=analogRead(topHat1);
  topHat2_val=analogRead(topHat2);
  thumbHat1_val=analogRead(thumbHat1);
  thumbHat2_val=analogRead(thumbHat2);
  triggerButtons_val=analogRead(triggerButtons);
  
  brakeButton_val=digitalRead(brakeButton);
  topSideButton_val=digitalRead(topSideButton);
  pinkyButton_val=digitalRead(pinkyButton);

  readoutString="TS1:" + String(thumbstick1_val)
      + ";TS2:" + String(thumbstick2_val)
      + ";topH1:" + String(topHat1_val)
      + ";topH2:" + String(topHat2_val)
      + ";thumbH1:" + String(thumbHat1_val)
      + ";thumbH2:" + String(thumbHat2_val)
      + ";trigger:" + String(triggerButtons_val)
      + ";brake:" + String(brakeButton_val)
      + ";topSide:" + String(topSideButton_val)
      + ";pinky:" + String(pinkyButton_val);

  Serial.println(readoutString);
  //Serial.println(getStringPackage());
  //requestCounter++;
  //if(requestCounter==requestCounterLimit) requestCounter=0;
}

// splits the readoutString into numbered packages containing at most 30 bytes with 26 (PACKAGE_MESSAGE_LENGTH) of those being from the readoutString and the rest being identifiers
String getStringPackage(){
  int start=requestCounter*(PACKAGE_MESSAGE_LENGTH+1);
  int end=requestCounter*(PACKAGE_MESSAGE_LENGTH+1)+(PACKAGE_MESSAGE_LENGTH+1);
  String retStr;
  retStr="["+String(requestCounter)+"]"+readoutString.substring(start,end)+"|";
  return retStr;
  }

void requestEvent(){
  String message = getStringPackage();
  Serial.println("ReqestEvent; Sending String:" + message + "\nrequestCounterLimit=" + String(requestCounterLimit));
  Wire.write("Hello");
  //Wire.write(getStringPackage().c_str());
  //Serial.println(getStringPackage().c_str());
  //increment counter and eventually reset it to 0
  requestCounter++;
  if(requestCounter==requestCounterLimit) requestCounter=0;
}
