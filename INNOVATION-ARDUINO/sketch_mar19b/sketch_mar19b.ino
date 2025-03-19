#define IN1    11
#define IN2    10
#define IN3    9
#define IN4    3
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// Variables for sensor data and filtering
Vector rawAccel, normAccel;
Vector rawGyro;
float pitch = 0, roll = 0; // Orientation angles
float alpha = 0.98; // Complementary filter coefficient
unsigned long lastTime = 0;

// Motor control function
void controlMotors(float pitch, float roll) {
  // Example logic: Adjust motor speed based on pitch and roll
  if (pitch > 10) {
    // Tilted forward: Move forward
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else if (pitch < -10) {
    // Tilted backward: Move backward
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  } else if (roll > 10) {
    // Tilted right: Turn right
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  } else if (roll < -10) {
    // Tilted left: Turn left
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else {
    // Level: Stop motors
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
}

// Calibration function
void calibrateMPU6050() {
  Serial.println("Calibrating MPU6050... Do not move the sensor!");
  delay(1000);
  mpu.calibrateGyro();
  mpu.setAccelOffsetX(0);
  mpu.setAccelOffsetY(0);
  mpu.setAccelOffsetZ(0);
  Serial.println("Calibration complete!");
}

void setup() {
  // Initialize motor control pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  // Initialize Serial communication
  Serial.begin(115200);
  Serial.println("Initialize MPU6050");

  // Initialize MPU6050
  while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }

  // Calibrate MPU6050
  calibrateMPU6050();

  // Check sensor settings
  checkSettings();
}

void checkSettings() {
  Serial.println();
  Serial.print(" * Sleep Mode:            ");
  Serial.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");
  Serial.print(" * Clock Source:          ");
  switch (mpu.getClockSource()) {
    case MPU6050_CLOCK_KEEP_RESET:     Serial.println("Stops the clock and keeps the timing generator in reset"); break;
    case MPU6050_CLOCK_EXTERNAL_19MHZ: Serial.println("PLL with external 19.2MHz reference"); break;
    case MPU6050_CLOCK_EXTERNAL_32KHZ: Serial.println("PLL with external 32.768kHz reference"); break;
    case MPU6050_CLOCK_PLL_ZGYRO:      Serial.println("PLL with Z axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_YGYRO:      Serial.println("PLL with Y axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_XGYRO:      Serial.println("PLL with X axis gyroscope reference"); break;
    case MPU6050_CLOCK_INTERNAL_8MHZ:  Serial.println("Internal 8MHz oscillator"); break;
  }
  Serial.print(" * Accelerometer:         ");
  switch (mpu.getRange()) {
    case MPU6050_RANGE_16G: Serial.println("+/- 16 g"); break;
    case MPU6050_RANGE_8G:  Serial.println("+/- 8 g"); break;
    case MPU6050_RANGE_4G:  Serial.println("+/- 4 g"); break;
    case MPU6050_RANGE_2G:  Serial.println("+/- 2 g"); break;
  }
  Serial.print(" * Accelerometer offsets: ");
  Serial.print(mpu.getAccelOffsetX());
  Serial.print(" / ");
  Serial.print(mpu.getAccelOffsetY());
  Serial.print(" / ");
  Serial.println(mpu.getAccelOffsetZ());
  Serial.println();
}

void loop() {
  // Read sensor data
  rawAccel = mpu.readRawAccel();
  rawGyro = mpu.readRawGyro();

  // Calculate time difference
  unsigned long currentTime = millis();
  float deltaTime = (currentTime - lastTime) / 1000.0; // Convert to seconds
  lastTime = currentTime;

  // Calculate pitch and roll from accelerometer
  float accelPitch = atan2(rawAccel.YAxis, sqrt(rawAccel.XAxis * rawAccel.XAxis + rawAccel.ZAxis * rawAccel.ZAxis)) * 180 / PI;
  float accelRoll = atan2(-rawAccel.XAxis, sqrt(rawAccel.YAxis * rawAccel.YAxis + rawAccel.ZAxis * rawAccel.ZAxis)) * 180 / PI;

  // Integrate gyroscope data
  pitch += rawGyro.YAxis * deltaTime;
  roll += rawGyro.XAxis * deltaTime;

  // Apply complementary filter
  pitch = alpha * (pitch + rawGyro.YAxis * deltaTime) + (1 - alpha) * accelPitch;
  roll = alpha * (roll + rawGyro.XAxis * deltaTime) + (1 - alpha) * accelRoll;

  // Print orientation data
  Serial.print("Pitch: ");
  Serial.print(pitch);
  Serial.print(" Roll: ");
  Serial.println(roll);

  // Control motors based on orientation
  controlMotors(pitch, roll);

  // Small delay for stability
  delay(10);
}