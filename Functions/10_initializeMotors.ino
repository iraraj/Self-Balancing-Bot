#include <Wire.h>

#define MPU6050_ADDR 0x68
#define PWMA 25
#define AIN1 26
#define AIN2 27
#define PWMB 14
#define BIN1 13
#define BIN2 33
#define STBY 32

int16_t Ax, Ay, Az;
int16_t Gx, Gy, Gz;
float Ax_g, Ay_g, Az_g;
float Gx_dps, Gy_dps, Gz_dps;
float accelAngle = 0;
unsigned long previousTime = 0;
float dt = 0;
float gyroBiasX = 0;
float gyroBiasY = 0;
float gyroBiasZ = 0;
float gyroAngle = 0;
float pitch = 0;
float filteredPitch = 0;

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
    Gx_dps = (Gx - gyroBiasX) / 131.0;
    Gy_dps = (Gy - gyroBiasY) / 131.0;
    Gz_dps = (Gz - gyroBiasZ) / 131.0;
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

void calibrateGyroscope()
{
    long sumX = 0;
    long sumY = 0;
    long sumZ = 0;

    for (int i = 0; i < 1000; i++)
    {
        readMPU6050();

        sumX += Gx;
        sumY += Gy;
        sumZ += Gz;

        delay(2);
    }

    gyroBiasX = sumX / 1000.0;
    gyroBiasY = sumY / 1000.0;
    gyroBiasZ = sumZ / 1000.0;
}

void calculateGyroAngle()
{
    gyroAngle += Gy_dps * dt;
}

void applyComplementaryFilter()
{
    pitch = 0.98 * gyroAngle + 0.02 * accelAngle;

    gyroAngle = pitch;
}

void applyAngleFilter()
{
    filteredPitch = 0.8 * filteredPitch + 0.2 * pitch;
}

void initializeMotors()
{
    pinMode(PWMA, OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);

    pinMode(PWMB, OUTPUT);
    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);

    pinMode(STBY, OUTPUT);

    digitalWrite(STBY, HIGH);

    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);

    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);

    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);
}

void setup()
{
    Serial.begin(115200);

    Wire.begin(21, 22);

    initializeMPU6050();

    initializeMotors();

    previousTime = micros();

    Serial.println("Keep the bot still...");
    delay(1000);

    calibrateGyroscope();

    Serial.println("Gyroscope calibrated");
}

void loop()
{
    calculateDt();

    readMPU6050();

    convertSensorData();

    calculateAccelAngle();

    calculateGyroAngle();

    applyComplementaryFilter();

    applyAngleFilter();

    Serial.print("Accel Angle: ");
    Serial.print(accelAngle);

    Serial.print("\tGyro Angle: ");
    Serial.print(gyroAngle);

    Serial.print("\tPitch: ");
    Serial.print(pitch);

    Serial.print("\tFiltered Pitch: ");
    Serial.println(filteredPitch);

    delay(100);
}
