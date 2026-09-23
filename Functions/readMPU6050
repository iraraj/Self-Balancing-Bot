#include <Wire.h>

#define MPU6050_ADDR 0x68
int16_t Ax, Ay, Az;
int16_t Gx, Gy, Gz;

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

void setup()
{
    Serial.begin(115200);
    Wire.begin(21, 22);
    initializeMPU6050();
    Serial.println("MPU6050 initialized");
}

void loop()
{
    readMPU6050();
    Serial.print("Ax: ");
    Serial.print(Ax);
    Serial.print("\tAy: ");
    Serial.print(Ay);
    Serial.print("\tAz: ");
    Serial.print(Az);
    Serial.print("\tGx: ");
    Serial.print(Gx);
    Serial.print("\tGy: ");
    Serial.print(Gy);
    Serial.print("\tGz: ");
    Serial.println(Gz);
    delay(100);
}
