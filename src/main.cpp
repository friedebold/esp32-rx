#include <Arduino.h>
#include "main.h"
#include "imu.h"
#include "coms.h"
#include "battery.h"
#include "motors.h"

float dt;
float global_thrust;

Adafruit_MPU6050 mpu;
IMUData imu_data;
Battery battery;
Remote remote_data;
PID pid;
Thrust thrust;
Correction correction;

void setup()
{
  Serial.begin(115200);
  setup_wifi();
  /* setup_imu();*/
  setup_battery();
  setup_motors();
}

void loop()
{
  set_clock();
  /* read_imu();*/
  read_battery();

  set_global_thrust();
  set_correction();
  get_total_thrust();
  run_motors();
  send_data();
}