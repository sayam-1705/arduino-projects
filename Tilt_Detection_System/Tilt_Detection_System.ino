#include <Wire.h>
#include <Servo.h>

// ADXL345 I2C Address
#define ADXL345_ADDRESS 0x53

// Pin Assignments for LEDs and Servo
const int LED_LEFT = 8;
const int LED_RIGHT = 9;
const int LED_FORWARD = 10;
const int LED_BACKWARD = 11;
const int SERVO_PIN = 6;

// Servo setup
Servo tiltServo;

void setup() {
  // Initialize Serial Monitor for debugging
  Serial.begin(9600);

  // Initialize I2C communication
  Wire.begin();
  setupADXL345();

  // Initialize LEDs
  pinMode(LED_LEFT, OUTPUT);
  pinMode(LED_RIGHT, OUTPUT);
  pinMode(LED_FORWARD, OUTPUT);
  pinMode(LED_BACKWARD, OUTPUT);

  // Attach the servo
  tiltServo.attach(SERVO_PIN);
}

void loop() {
  int16_t x, y, z;
  readADXL345(x, y, z);

  // Calculate tilt angles
  float angleX = atan2(y, sqrt(x * x + z * z)) * 180 / PI;
  float angleY = atan2(x, sqrt(y * y + z * z)) * 180 / PI;

  // Print values for debugging
  Serial.print("Angle X: "); Serial.print(angleX);
  Serial.print(" | Angle Y: "); Serial.println(angleY);

  // LED and Servo Logic
  controlTiltOutput(angleX, angleY);

  delay(100);
}

// Function to set up the ADXL345 sensor
void setupADXL345() {
  Wire.beginTransmission(ADXL345_ADDRESS);
  Wire.write(0x2D);  // Power control register
  Wire.write(0x08);  // Set to Measurement Mode
  Wire.endTransmission();
}

// Function to read data from ADXL345
void readADXL345(int16_t &x, int16_t &y, int16_t &z) {
  Wire.beginTransmission(ADXL345_ADDRESS);
  Wire.write(0x32); // Starting with register 0x32 (X-axis data)
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345_ADDRESS, 6, true);

  x = Wire.read() | (Wire.read() << 8);
  y = Wire.read() | (Wire.read() << 8);
  z = Wire.read() | (Wire.read() << 8);
}

// Function to control LEDs and servo based on tilt
void controlTiltOutput(float angleX, float angleY) {
  // Reset LED states
  digitalWrite(LED_LEFT, LOW);
  digitalWrite(LED_RIGHT, LOW);
  digitalWrite(LED_FORWARD, LOW);
  digitalWrite(LED_BACKWARD, LOW);

  // Determine LED and servo behavior based on angle thresholds
  if (angleX > 20) {
    digitalWrite(LED_RIGHT, HIGH);   // Turn on Right LED for right tilt
    tiltServo.write(90 + angleX);    // Turn servo to the right
  } else if (angleX < -20) {
    digitalWrite(LED_LEFT, HIGH);    // Turn on Left LED for left tilt
    tiltServo.write(90 + angleX);    // Turn servo to the left
  } else if (angleY > 20) {
    digitalWrite(LED_FORWARD, HIGH); // Turn on Forward LED for forward tilt
  } else if (angleY < -20) {
    digitalWrite(LED_BACKWARD, HIGH); // Turn on Backward LED for backward tilt
  } else {
    tiltServo.write(90); // Center servo when flat
  }
}
