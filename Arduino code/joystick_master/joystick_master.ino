#include <Wire.h>
#include "btn_data_def.h"
#include "Joystick.h"

// the I2C address of the slave arduino in the joystick grip
#define SLAVE_ADDR 9 
// amount of buttons on the joystick
#define BTN_AMOUNT 8
// only 2 hat switches supported but I want to use 3. Therefore I actually have 12 buttons instead of 8
#define HAT_SWITCH_AMOUNT 2


#define X_AXIS_PIN A4
#define Y_AXIS_PIN A5
int xAxis, yAxis;
bool invertYAxis = true;
bool invertXAxis = true;

// data received from slave
Btn_Meta_t msgBuffer = {0};
// hidReportId, joystickType, amount of buttons, hatswitchcount, XAxis, YAxis, ZAxis, RxAxis, RyAxis, RzAxis, Rudder, Throttle, Accelerator, Brake, Steering
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_JOYSTICK, BTN_AMOUNT+4, HAT_SWITCH_AMOUNT, true, true, false, false, false, false, false, false, false, false, false);

// [0 brake, 1 pinky, 2 trigger0, 3 trigger1,
// 4 thumbHatPress, 5 topSide, 6 joystickPress, 7 topLeftButton]
int currentBtnState[BTN_AMOUNT];
//int lastButtonState[BTN_AMOUNT];


// 0 top, 1 right, 2 down, 3 left
int topLHat[4], topRHat[4], thumbHat[4];

void setup() {
  Wire.begin();
  Serial.begin(9600);
  xAxis=0;
  yAxis=0;
  // init button arrays
  for(int i=0;i<BTN_AMOUNT;++i){
     //lastButtonState[i]=0; 
     currentBtnState[i]=0;
  }
  for(int i=0;i<4;++i){
    topLHat[i]=0;
    topRHat[i]=0;
    thumbHat[i]=0;
  }
  Serial.println("I2C Joystick Master Start");
  Joystick.begin();
}

void loop() {
  getSlaveData();
  mapDataToArray();
  xAxis=analogRead(X_AXIS_PIN);
  if(invertXAxis){
    xAxis = map(xAxis, 0, 1023, 1023, 0);
  }
  yAxis=analogRead(Y_AXIS_PIN);
  if(invertYAxis){
    yAxis = map(yAxis, 0, 1023, 1023, 0);
  }
  // send button states
  for(int i=0;i<BTN_AMOUNT;++i){
    Joystick.setButton(i, currentBtnState[i]);
  }
  // top left hatswitch is used as normal buttons
  for(int i=BTN_AMOUNT;i<(BTN_AMOUNT+4);++i){
    Joystick.setButton(i, topLHat[i-BTN_AMOUNT]);
  }
  // send hatswitch states
  Joystick.setHatSwitch(0, hatSwitchToHeading(topRHat));
  Joystick.setHatSwitch(1, hatSwitchToHeading(thumbHat));
  // set axis
  Joystick.setXAxis(xAxis);
  Joystick.setYAxis(yAxis);
}

int hatSwitchToHeading(int* hatSwitchArray){
  for(int i=0;i<4;++i){
    if(hatSwitchArray[i]){
      return (i*90);
    }
  }
  return -1;
}

void mapDataToArray(){
  // index button mapping
  // [0 brake, 1 pinky, 2 trigger0, 3 trigger1,
  // 4 thumbHatPress, 5 topSide, 6 joystickPress, 7 topLeftButton]

  // single pin buttons
  // ===============================================================
  
  // pinky and brake are accidentally swapped on the slave arduino
  currentBtnState[0]=invBtnState(msgBuffer.data.pinkyButton_val);
  currentBtnState[1]=invBtnState(msgBuffer.data.brakeButton_val);
  currentBtnState[5]=invBtnState(msgBuffer.data.topSideButton_val);

  
  // resistor ladder buttons
  // ===============================================================
  /**
   * Calibration values:
   * 
   * Thumb hat:
   *         ^ thu2:684
   *         |
   *    <-  thu2:0  -> thu1:513
   * thu1:0  |
   *         v thu2:512
   *         
   * Trigger:
   * trigger half pressed: 512
   * trigger fully pressed: 0
   * 
   * Faceplate:
   * topLeftButton=topHat1:0
   * topRightButton=topHat2:0
   * 
   *        top left hat switch                   top right hat switch
   * 
   *              ^ topHat2:882                     ^ topHat2:513
   *              |                                 |
   *        <-        -> topHat1:684          <-        -> topHat1:822
   * topHat1:513  |                     topHat1:770 |
   *              v tophat2: 771                    v topHat2: 685
   *              
   *              
   */             
  // trigger
  int trg = msgBuffer.data.triggerButtons_val;
  currentBtnState[2]=(trg<1000);
  currentBtnState[3]=(trg<256);
  
  int th1 = msgBuffer.data.topHat1_val;
  currentBtnState[7] = (th1<256);
  topLHat[3]=(th1>256 && th1<600);
  topLHat[1]=(th1>600 && th1<720);
  topRHat[3]=(th1>720 && th1<800);
  topRHat[1]=(th1>800 && th1<1000);

  int th2 = msgBuffer.data.topHat2_val;
  currentBtnState[6]=(th2<256);
  topRHat[0]=(th2>256 && th2<600);
  topRHat[2]=(th2>600 && th2<720);
  topLHat[2]=(th2>720 && th2<800);
  topLHat[0]=(th2>800 && th2<1000);

  // thumbHat
  int thu1 = msgBuffer.data.thumbHat1_val;
  int thu2 = msgBuffer.data.thumbHat2_val;
  currentBtnState[4] = (thu2<256);
  thumbHat[2] = (thu2>256 && thu2<600);
  thumbHat[0] = (thu2>600 && thu2<1000);
  thumbHat[3] = (thu1<256);
  thumbHat[1] = (thu1>256 && thu1<1000);
}

int invBtnState(int state){
  if(state) return 0;
  return 1;
}

void getSlaveData(){
  Serial.println("requesting data");
  uint32_t dataReceived = 0;
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
    }
}
