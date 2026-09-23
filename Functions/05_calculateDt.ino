#include <Wire.h>

#define MPU6050_ADDR 0x68
int16_t Ax, Ay, Az;
int16_t Gx, Gy, Gz;
float Ax_g, Ay_g, Az_g;
float Gx_dps, Gy_dps, Gz_dps;
float accelAngle = 0;
unsigned long previousTime = 0;
float dt = 0;

void initializeMPU6050()
{
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x6B);
    Wire.write(0x00);
    Wire.endTransmission();
}

void readMPU6050()
{
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_ADDR, 14);
    Ax = (Wire.read() << 8) | Wire.read();
    Ay = (Wire.read() << 8) | Wire.read();
    Az = (Wire.read() << 8) | Wire.read();
    // Skip temperature
    Wire.read();
    Wire.read();
    Gx = (Wire.read() << 8) | Wire.read();
    Gy = (Wire.read() << 8) | Wire.read();
    Gz = (Wire.read() << 8) | Wire.read();
}

void convertSensorData()
{
    Ax_g = Ax / 16384.0;
    Ay_g = Ay / 16384.0;
    Az_g = Az / 16384.0;
    Gx_dps = Gx / 131.0;
    Gy_dps = Gy / 131.0;
    Gz_dps = Gz / 131.0;
}

void calculateAccelAngle()
{
    accelAngle = atan2(Ax_g, Az_g) * 180.0 / PI;
}

void calculateDt()
{
    unsigned long currentTime = micros();

    dt = (currentTime - previousTime) / 1000000.0;

    previousTime = currentTime;
}

void setup()
{
    Serial.begin(115200);
    Wire.begin(21, 22);
    initializeMPU6050();
    previousTime = micros();
    Serial.println("MPU6050 initialized");
}

void loop()
{
    calculateDt();

    readMPU6050();

    convertSensorData();

    calculateAccelAngle();

    Serial.print("dt: ");
    Serial.println(dt);

    delay(100);
}
