#include <Wire.h>
#include <MPU6050.h>

// Motor pins
#define MOTOR_XP1 2  // Positive X motor 1
#define MOTOR_XP2 3  // Positive X motor 2
#define MOTOR_XN1 7  // Negative X motor 1
#define MOTOR_XN2 5  // Negative X motor 2
#define MOTOR_YP1 8  // Positive Y motor 1
#define MOTOR_YP2 9  // Positive Y motor 2
#define MOTOR_YN1 6  // Negative Y motor 1
#define MOTOR_YN2 4  // Negative Y motor 2

// MPU6050 Configuration
MPU6050 mpu;
const float TILT_THRESHOLD = 15.0;  // Degrees threshold for motor activation

void setup() {
  Serial.begin(115200);
  
  // Initialize all motor pins
  pinMode(MOTOR_XP1, OUTPUT);
  pinMode(MOTOR_XP2, OUTPUT);
  pinMode(MOTOR_XN1, OUTPUT);
  pinMode(MOTOR_XN2, OUTPUT);
  pinMode(MOTOR_YP1, OUTPUT);
  pinMode(MOTOR_YP2, OUTPUT);
  pinMode(MOTOR_YN1, OUTPUT);
  pinMode(MOTOR_YN2, OUTPUT);
  
  // Ensure all motors are off initially
  allMotorsOff();

  Serial.println("Initialize MPU6050");
  
  // Initialize MPU6050
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  
  // Print sensor settings
  checkSettings();
}

void checkSettings() {
  Serial.println("\nMPU6050 Settings:");
  
  Serial.print(" * Accelerometer range: ");
  switch(mpu.getRange()) {
    case MPU6050_RANGE_16G: Serial.println("±16g"); break;
    case MPU6050_RANGE_8G:  Serial.println("±8g");  break;
    case MPU6050_RANGE_4G:  Serial.println("±4g");  break;
    case MPU6050_RANGE_2G:  Serial.println("±2g");  break;
  }
  
  Serial.println();
}

void loop() {
  // Read normalized accelerometer data
  Vector normAccel = mpu.readNormalizeAccel();
  
  // Calculate tilt angles (in degrees)
  float roll = atan2(normAccel.YAxis, normAccel.ZAxis) * 180.0 / PI;   // X-axis tilt
  float pitch = atan2(normAccel.XAxis, normAccel.ZAxis) * 180.0 / PI;  // Y-axis tilt
  
  // Print tilt angles
  Serial.print("Roll (X): ");
  Serial.print(roll);
  Serial.print("°\tPitch (Y): ");
  Serial.print(pitch);
  Serial.println("°");
  
  // Control motors based on tilt
  controlMotors(roll, pitch);
  
  delay(100);
}

void controlMotors(float roll, float pitch) {
  // First turn off all motors
  allMotorsOff();
  
  // X-axis tilt control (Roll)
  if (roll > TILT_THRESHOLD) {
    Serial.println("Tilted right (Positive X) - Activating XP motors");
    digitalWrite(MOTOR_XP1, HIGH);
    digitalWrite(MOTOR_XP2, HIGH);
  } 
  else if (roll < -TILT_THRESHOLD) {
    Serial.println("Tilted left (Negative X) - Activating XN motors");
    digitalWrite(MOTOR_XN1, HIGH);
    digitalWrite(MOTOR_XN2, HIGH);
  }
  
  // Y-axis tilt control (Pitch)
  if (pitch > TILT_THRESHOLD) {
    Serial.println("Tilted forward (Positive Y) - Activating YP motors");
    digitalWrite(MOTOR_YP1, HIGH);
    digitalWrite(MOTOR_YP2, HIGH);
  } 
  else if (pitch < -TILT_THRESHOLD) {
    Serial.println("Tilted backward (Negative Y) - Activating YN motors");
    digitalWrite(MOTOR_YN1, HIGH);
    digitalWrite(MOTOR_YN2, HIGH);
  }
}

void allMotorsOff() {
  digitalWrite(MOTOR_XP1, LOW);
  digitalWrite(MOTOR_XP2, LOW);
  digitalWrite(MOTOR_XN1, LOW);
  digitalWrite(MOTOR_XN2, LOW);
  digitalWrite(MOTOR_YP1, LOW);
  digitalWrite(MOTOR_YP2, LOW);
  digitalWrite(MOTOR_YN1, LOW);
  digitalWrite(MOTOR_YN2, LOW);
}
