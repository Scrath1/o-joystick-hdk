#ifndef BTN_DATA_H
#define BTN_DATA_H
#include "Arduino.h"

enum EncoderDirection {
    NONE = 0,
    LEFT = 1,
    RIGHT = 2,
};

union Btn_Data_t{
    struct __attribute__((packed)){
        uint8_t topHatLeft; // up to 5 directions of faceplate left hat switch
        uint8_t topHatRight; // up to 5 directions of faceplate right hat switch
        uint16_t topStickX; // x-axis of faceplate joystick
        uint16_t topStickY; // y-axis of faceplate joystick
        uint8_t topButtons; // thumbstick down, top side button, top button, trigger 1, trigger 2
        uint8_t lowerButtons; // brake, pinky button, 5 thumbhat directions
        EncoderDirection encoder;
    } data;

    unsigned char rawData[sizeof(data)];
};

#endif // BTN_DATA_H