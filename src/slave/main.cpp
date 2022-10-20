#include <Arduino.h>

#include "../btn_data.h"

#define SAMPLE_PERIOD 2000
#define CLK_PIN 2
#define LATCH_PIN 3
#define PISO1_PIN 0
#define PISO2_PIN 1

uint8_t piso1 = 0;
uint8_t piso2 = 0;

void setup() {
    Serial.begin(9600);
    while (!Serial)
        ;

    pinMode(CLK_PIN, OUTPUT);
    pinMode(LATCH_PIN, OUTPUT);
    pinMode(PISO1_PIN, INPUT);
    pinMode(PISO2_PIN, INPUT);
}

void readPisos() {
    digitalWrite(CLK_PIN, HIGH);  // preset clock to retrieve first bit
    digitalWrite(LATCH_PIN, HIGH);   // disable input latching and enable shifting
    for (uint8_t i = 0; i < 8; ++i) {
        digitalWrite(CLK_PIN, HIGH);
        piso1 |= digitalRead(PISO1_PIN) << i; // LSBFIRST
        piso2 |= digitalRead(PISO2_PIN) << i;
        digitalWrite(CLK_PIN, LOW);
    }
    digitalWrite(LATCH_PIN, LOW);  // disable shifting and enable input latching
}

void loop() {
    readPisos();
    Serial.print("Piso 1: ");
    Serial.print(piso1, BIN);
    Serial.print("\r\nPiso 2: ");
    Serial.print(piso2, BIN);
    Serial.print("\r\n");
    delay(SAMPLE_PERIOD);
    piso1=0;
}