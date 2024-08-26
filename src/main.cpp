#include <Arduino.h>
#include "main.h"
#include "imu.h"
#include "coms.h"
#include "battery.h"
#include "motors.h"

float dt;

float roll_target = 0.0;
float global_thrust;

Adafruit_MPU6050 mpu;
IMUData imu_data;
BatteryData battery_data;
Remote remote_data;
PID pid_data;
Thrust thrust;
Correction correction;

void setup()
{
  Serial.begin(115200);
  setup_wifi();
  /* setup_imu();
  setup_battery(); */
  setup_motors();
}

float roll_error = 0.0;
float roll_error_old = 0.0;

void set_correction()
{
  roll_target = remote_data.right.x * 30;
  roll_error = roll_target - imu_data.roll;

  correction.roll.p = roll_error * pid_data.p;                           // 0.04
  correction.roll.i = correction.roll.i + roll_error * dt * pid_data.i;  // 0.01;
  correction.roll.d = ((roll_error - roll_error_old) / dt) * pid_data.d; // 0.02

  roll_error_old = roll_error;
}

void loop()
{
  set_clock();
  /* read_imu();
  read_battery(); */

  set_global_thrust();
  set_correction();
  get_total_thrust();

  Serial.print(thrust.left);
  Serial.print("\t");
  Serial.print(thrust.right);
  Serial.print("\t");
  Serial.print(thrust.front);
  Serial.print("\t");
  Serial.print(thrust.back);
  Serial.println();

  run_motors();
}