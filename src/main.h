#ifndef MAIN_H
#define MAIN_H
#include <Adafruit_MPU6050.h>

struct Battery
{
    float lowestBat;
    float cBat;
    float vBat1;
    float vBat2;
    float vBat3;
};

struct IMU
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
extern IMU imu;
extern Battery battery;
extern PID pid;
extern Correction correction;
extern Thrust thrust;

#endif