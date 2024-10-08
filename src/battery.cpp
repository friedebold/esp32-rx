#include <Arduino.h>
#include <battery.h>

const int VBAT1_PIN = 39;
const int VBAT2_PIN = 36;
const int VBAT3_PIN = 34;
const float VCC = 3.31;

void setup_battery()
{
    pinMode(VBAT1_PIN, INPUT);
    pinMode(VBAT2_PIN, INPUT);
    pinMode(VBAT3_PIN, INPUT);
}

void read_battery()
{
    int vBat1Input = analogRead(VBAT1_PIN);
    float vBat1Voltage = vBat1Input * (VCC / 4095.0);
    float vBat1 = vBat1Voltage; //* 1.368627451;

    int vBat2Input = analogRead(VBAT2_PIN);
    float vBat2Voltage = vBat2Input * (VCC / 4095.0);
    float vBat2 = vBat2Voltage * 1.5936073059;

    int vBat3Input = analogRead(VBAT3_PIN);
    float vBat3Voltage = vBat3Input * (VCC / 4095.0);
    float vBat3 = vBat3Voltage * 1.4936170213;

    battery.lowestBat = min(vBat1, min(vBat2, vBat3));
    battery.vBat1 = vBat1;
    battery.vBat2 = vBat2;
    battery.vBat3 = vBat3;
}