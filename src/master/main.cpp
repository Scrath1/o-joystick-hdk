#include <Arduino.h>
#include <Joystick.h>
#include <Wire.h>
#include "../btn_data.h"

// I2C Address of Slave
#define SLAVE_ADDR 9 

#define BTN_AMOUNT 8
// only 2 hat switches supported but I want to use 3. Therefore I actually have 12 buttons instead of 8
#define HAT_SWITCH_AMOUNT 2
#define X_AXIS_PIN A5
#define Y_AXIS_PIN A4

#define DEBUG_INFO false

// data from slave
Btn_Data_t jstData = {0};

uint16_t xAxis{0}, yAxis{0};
const uint16_t xCenter = 492;
const uint16_t yCenter = 524;
const int deadzone = 10;
const bool invertXAxis = false;
const bool invertYAxis = false;
const bool overwriteXCenter = true;
const bool overwriteYCenter = true;
const bool invertRxAxis = true;
const bool invertRyAxis = true;

// hidReportId, joystickType, amount of buttons, hatswitchcount, XAxis, YAxis, ZAxis, RxAxis, RyAxis, RzAxis, Rudder, Throttle, Accelerator, Brake, Steering
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_JOYSTICK, BTN_AMOUNT+4, HAT_SWITCH_AMOUNT, true, true, false, true, true, false, false, false, false, false, false);

void setup() {
  Serial.begin(9600);
  if(DEBUG_INFO) Serial.println("Starting Joystick");
  Joystick.begin(false);
}

void loop() {
  // put your main code here, to run repeatedly:
}