#ifndef MAIN_H
#define MAIN_H
#include <Adafruit_MPU6050.h>

struct BatteryData
{
    int isBatConnected;
    float vBat1;
    float vBat2;
    float vBat3;
    float cBat;
};

struct IMUData
{
    float pitch;
    float roll;
    float yaw;
};

struct Joystick
{
    float x;
    float y;
};

struct Remote
{
    Joystick left;
    Joystick right;
    bool left_button;
    bool right_button;
};

struct PID
{
    float p;
    float i;
    float d;
};

struct Thrust
{
    float left;
    float right;
    float front;
    float back;
};

struct Correction
{
    PID pitch;
    PID roll;
    PID yaw;
};

extern float dt;
extern float roll_target;
extern float global_thrust;

extern Adafruit_MPU6050 mpu;
extern Remote remote_data;
extern IMUData imu_data;
extern BatteryData battery_data;
extern PID pid_data;
extern Correction correction;
extern Thrust thrust;

#endif