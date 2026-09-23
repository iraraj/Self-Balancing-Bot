#include <Wire.h>

#define MPU6050_ADDR 0x68


void initializeMPU6050()
{
    Wire.beginTransmission(MPU6050_ADDR);

    Wire.write(0x6B);

    Wire.write(0x00);

    Wire.endTransmission();
}


void setup()
{
    Serial.begin(115200);

    Wire.begin(21, 22);

    initializeMPU6050();

    Serial.println("MPU6050 initialized");
}


void loop()
{
}
