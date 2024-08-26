#include <Arduino.h>
#include <motors.h>
#include <ESP32Servo.h>

int frontESCpin = 18;
int rightESCpin = 17;
int backESCpin = 5;
int leftESCpin = 16;
Servo FrontESC;
Servo RightESC;
Servo BackESC;
Servo LeftESC;

void setup_motors()
{
    FrontESC.attach(frontESCpin, 1000, 2000);
    RightESC.attach(rightESCpin, 1000, 2000);
    BackESC.attach(backESCpin, 1000, 2000);
    LeftESC.attach(leftESCpin, 1000, 2000);
    FrontESC.write(0);
    RightESC.write(0);
    BackESC.write(0);
    LeftESC.write(0);
}

void set_global_thrust()
{
    float g_thrust = 0.0;
    float thrust_diff = 0.0;
    const float thrust_factor = 0.01;

    // Map remote to control
    float thrust_value = remote_data.left.y;
    float interrupt_value = remote_data.left.x;
    bool armed = remote_data.left_button;

    // Set global thrust
    if (thrust_value > 0.05 || thrust_value < -0.05)
    {
        thrust_diff = thrust_value * thrust_factor;
    }
    g_thrust = min(max(float(0), global_thrust + thrust_diff), float(100));

    // Fail safe
    if (armed == 0 || (interrupt_value > 0.3 || interrupt_value < -0.3))
    {
        g_thrust = 0;
    }
    global_thrust = g_thrust;
}

void get_total_thrust()
{
    float thrust_right = min(max(float(0.0), global_thrust + correction.roll.p + correction.roll.i + correction.roll.d), float(180.0));
    float thrust_left = min(max(float(0.0), global_thrust - correction.roll.p - correction.roll.i - correction.roll.d), float(180.0));
    float thrust_front = min(max(float(0.0), global_thrust), float(180.0));
    float thrust_back = min(max(float(0.0), global_thrust), float(180.0));

    /*  Serial.print(imu_data.roll);
     Serial.print("\t");
     Serial.print(battery_data.vBat1);
     Serial.print("\t");
     Serial.print(battery_data.vBat2);
     Serial.print("\t");
     Serial.print(battery_data.vBat3);
     Serial.print("\t");
     Serial.print(battery_data.cBat);
     Serial.println(); */

    if (global_thrust == 0.0                                                                       // Keep correction 0 if unarmed
        || imu_data.roll > 60                                                                      // Positive rollover protection
        || imu_data.roll < -60                                                                     // Negative rollover protection
        || ((battery_data.vBat1 < 2.6 || battery_data.vBat2 < 2.6 || battery_data.vBat3 < 2.6)     // Low Battery protection
            && (battery_data.vBat1 > 0.0 || battery_data.vBat2 < 0.0 || battery_data.vBat3 < 0.0)) // Battery disconnected
        || battery_data.cBat < -25.0)                                                              // High Current protection

    {
        global_thrust = 0.0;
        thrust_left = 0.0;
        thrust_right = 0.0;
        thrust_front = 0.0;
        thrust_back = 0.0;
        correction.roll.i = 0.0;
    }
    thrust.left = thrust_left;
    thrust.right = thrust_right;
    thrust.front = thrust_front;
    thrust.back = thrust_back;
}

void run_motors()

{
    FrontESC.write(thrust.front);
    RightESC.write(thrust.left);
    BackESC.write(thrust.back);
    LeftESC.write(thrust.right);
}