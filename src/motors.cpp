#include <Arduino.h>
#include <motors.h>
#include "DShotRMT.h"

const auto MOTOR01_PIN = 17;
const auto MOTOR02_PIN = 5;
const auto MOTOR03_PIN = 18;
const auto MOTOR04_PIN = 19;

const auto DSHOT_MODE = DSHOT300;

DShotRMT motor01(MOTOR01_PIN, RMT_CHANNEL_0);
DShotRMT motor02(MOTOR02_PIN, RMT_CHANNEL_1);
DShotRMT motor03(MOTOR03_PIN, RMT_CHANNEL_2);
DShotRMT motor04(MOTOR04_PIN, RMT_CHANNEL_3);

const auto FAILSAFE_THROTTLE = 999;
const auto INITIAL_THROTTLE = 48;

float throttle_value = INITIAL_THROTTLE;

void setup_motors()
{
    motor01.begin(DSHOT_MODE);
    motor02.begin(DSHOT_MODE);
    motor03.begin(DSHOT_MODE);
    motor04.begin(DSHOT_MODE);
}

float clamp(float value, float min_val, float max_val)
{
    return min(max(min_val, value), max_val);
}

void set_global_thrust()
{
    // Read Joystick values
    float raw_thrust_value = remote_data.left.y;
    bool raw_is_armed = remote_data.left_button;
    bool is_significant_change = raw_thrust_value > 0.05 || raw_thrust_value < -0.05;

    if (raw_is_armed == 1 &&
        imu_data.roll > -60 && imu_data.roll < 60 && // Roll over protection
        battery.lowestBat > 2.6                      // Low Battery protection
    )
    {
        float thrust_diff = is_significant_change == 1 ? raw_thrust_value * 0.1 : 0;
        global_thrust = clamp(global_thrust + thrust_diff, float(65), float(2047));
        return;
    }
    global_thrust = 0;
}

float roll_error_old = 0.0;
float roll_error = 0.0;

void set_correction()
{
    float roll_target = 0.0;
    roll_error = roll_target - imu_data.roll;

    correction.roll.p = roll_error * pid.p;                           // 0.04
    correction.roll.i = correction.roll.i + roll_error * dt * pid.i;  // 0.01;
    correction.roll.d = ((roll_error - roll_error_old) / dt) * pid.d; // 0.02

    roll_error_old = roll_error;
}

void get_total_thrust()
{
    if (global_thrust > 0.0)
    {
        thrust.right = clamp(global_thrust + correction.roll.p + correction.roll.i + correction.roll.d, float(0), float(2047));
        thrust.left = clamp(global_thrust - correction.roll.p - correction.roll.i - correction.roll.d, float(0), float(2047));
        thrust.front = clamp(global_thrust, float(0), float(2047));
        thrust.back = clamp(global_thrust, float(0), float(2047));
        return;
    }
    thrust.right = 0.0;
    thrust.left = 0.0;
    thrust.front = 0.0;
    thrust.back = 0.0;
    correction.roll.i = 0.0;
}

void run_motors()
{
    motor01.sendThrottleValue(thrust.right);
    motor02.sendThrottleValue(thrust.left);
    motor03.sendThrottleValue(thrust.front);
    motor04.sendThrottleValue(thrust.back);
}