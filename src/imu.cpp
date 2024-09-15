#include <Arduino.h>
#include <imu.h>

float micros_old = 0.0;

void set_clock()
{
    dt = (micros() - micros_old) / 1000000.0;
    micros_old = micros();
}

void setup_imu()
{
    // IMU
    while (!mpu.begin())
    {
        Serial.println("Failed to find MPU6050 chip");
        delay(1000);
    }
    mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
    mpu.setMotionDetectionThreshold(1);
    mpu.setMotionDetectionDuration(20);
    mpu.setInterruptPinLatch(true);
    mpu.setInterruptPinPolarity(true);
    mpu.setMotionInterrupt(true);
}

const float GYRO_FACTOR = 0.98;
const float ACC_FACTOR = 1 - GYRO_FACTOR;
void read_imu()
{
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Calibrate Accelerometer
    const float acc_x = a.acceleration.x - 0.97;
    const float acc_y = a.acceleration.y - 0.12;
    const float acc_z = a.acceleration.z + 0.17;

    // Calculate Accelerometer Pitch and Roll
    const float a_pitch = -atan2(acc_y / 9.8, acc_z / 9.8) * RAD_TO_DEG;
    const float a_roll = atan2(acc_x / 9.8, acc_z / 9.8) * RAD_TO_DEG;

    // Calculate Gyro Pitch and Roll
    // g_pitch = g_pitch - g.gyro.x * dt * RAD_TO_DEG;
    // g_roll = g_roll - g.gyro.y * dt * RAD_TO_DEG;

    // Calculate Pitch and Roll
    imu.pitch = GYRO_FACTOR * (imu.pitch - g.gyro.x * dt * RAD_TO_DEG) + ACC_FACTOR * a_pitch;
    imu.roll = GYRO_FACTOR * (imu.roll - g.gyro.y * dt * RAD_TO_DEG) + ACC_FACTOR * a_roll;
    imu.yaw = 0.0;
};
