#include <Arduino.h>
#include <battery.h>

const int VBAT1_PIN = A4;
const int VBAT2_PIN = A7;
const int VBAT3_PIN = A6;
const int CBAT_PIN = A3;
const float VCC = 3.31;

void setup_battery()
{
    pinMode(CBAT_PIN, INPUT);
}

void read_battery()
{
    int vBat1Input = analogRead(VBAT1_PIN);
    float vBat1Voltage = vBat1Input * (VCC / 4095.0);
    float vBat1 = vBat1Voltage * 1.4710743802;

    int vBat2Input = analogRead(VBAT2_PIN);
    float vBat2Voltage = vBat2Input * (VCC / 4095.0);
    float vBat2 = vBat2Voltage * 1.89941145;

    int vBat3Input = analogRead(VBAT3_PIN);
    float vBat3Voltage = vBat3Input * (VCC / 4095.0);
    float vBat3 = vBat3Voltage * 1.978503229;

    int cBatInput = analogRead(CBAT_PIN);
    float cBatVoltage = cBatInput * (VCC / 4095.0);
    float cBat = (cBatVoltage - VCC / 2.0 + 0.162) * -25;

    int isBatConnected = 0;
    if (vBat1 >= 2.5 && vBat2 >= 2.5 && vBat3 >= 2.5)
    {
        isBatConnected = 1;
    };
    battery_data.isBatConnected = isBatConnected;
    battery_data.vBat1 = vBat1;
    battery_data.vBat2 = vBat2;
    battery_data.vBat3 = vBat3;
    battery_data.cBat = cBat;
}