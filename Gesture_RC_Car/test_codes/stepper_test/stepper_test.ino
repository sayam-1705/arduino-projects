// -------------------------------------------
// Code to Test Stepper Motor + ULN2003
// Rotates stepper motor 90° clockwise and counterclockwise with serial feedback
#include <Stepper.h>

const int stepsPerRev = 2048;              // Number of steps per revolution for 28BYJ-48
Stepper stepper(stepsPerRev, 2, 4, 3, 5);  // Initialize stepper with IN1-IN4 connected to pins 2, 4, 3, 5

int motorSpeed = 10;                  // Default speed (RPM)
int stepIncrement = stepsPerRev / 4;  // Default step increment (90° rotation)

void setup() {
  Serial.begin(9600);            // Initialize serial communication for debugging
  stepper.setSpeed(motorSpeed);  // Set motor speed to default RPM
  Serial.println("Stepper Motor Test Initialized...");
}

void loop() {
  // Rotate clockwise 90° (stepsPerRev/4)
  stepper.step(stepIncrement);
  Serial.print("Rotating clockwise: ");
  Serial.print(stepIncrement);
  Serial.println(" steps");
  delay(1000);  // Wait for a second

  // Rotate counterclockwise 90° (-stepsPerRev/4)
  stepper.step(-stepIncrement);
  Serial.print("Rotating counterclockwise: ");
  Serial.print(stepIncrement);
  Serial.println(" steps");
  delay(1000);  // Wait for a second
}
