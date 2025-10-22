// -------------------------------------------
// Basic Integration Code
// Combines PS2 Joystick with DC Motor and Stepper Motor (ULN2003 + 28BYJ-48)
#include <Stepper.h>

// Define stepper motor setup (28BYJ-48)
const int stepsPerRev = 2048;
Stepper stepper(stepsPerRev, 2, 4, 3, 5);  // IN1-IN4 connected to pins 2,4,3,5

// DC Motor control pin (connected via 2N2222 transistor)
const int motorPin = 9;

// Thresholds for joystick deadzones
const int deadzoneLow = 450;
const int deadzoneHigh = 570;

void setup() {
  Serial.begin(9600);         // Start Serial Monitor for debugging
  pinMode(motorPin, OUTPUT);  // DC motor output pin
  stepper.setSpeed(10);       // RPM for stepper motor
  Serial.println("Gesture RC Car Integration Started");
}

void loop() {
  int xVal = analogRead(A0);  // Joystick X-axis
  int yVal = analogRead(A1);  // Joystick Y-axis

  // --- Debug Print Joystick Values ---
  Serial.print("X: ");
  Serial.print(xVal);
  Serial.print(" | Y: ");
  Serial.println(yVal);

  // --- DC Motor Control (Forward / Stop / Backward simulation) ---
  if (yVal < deadzoneLow) {
    digitalWrite(motorPin, HIGH);  // Move Forward
    Serial.println("DC Motor: FORWARD");
  } else if (yVal > deadzoneHigh) {
    digitalWrite(motorPin, HIGH);  // Move Backward (same as forward in this simple setup)
    Serial.println("DC Motor: BACKWARD");
  } else {
    digitalWrite(motorPin, LOW);  // Stop
    Serial.println("DC Motor: STOPPED");
  }

  // --- Stepper Motor Control (Left / Right Steering) ---
  if (xVal < deadzoneLow) {
    stepper.step(-25);  // Turn Left
    Serial.println("Stepper: LEFT");
  } else if (xVal > deadzoneHigh) {
    stepper.step(25);  // Turn Right
    Serial.println("Stepper: RIGHT");
  } else {
    // No stepper movement in deadzone
  }

  delay(150);  // Short delay to debounce joystick input
}
