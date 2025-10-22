// -------------------------------------------
// Optimized Integration Logic for Gesture-Controlled RC Car
#include <Stepper.h>  // Include Stepper library for ULN2003 + 28BYJ-48

// Pin configuration
const int motorPin = 9;    // DC motor control pin (via 2N2222 transistor)
const int ledPin = 7;      // LED headlights
const int buttonPin = 8;   // Joystick push-button (SW)
const int DEADZONE = 100;  // Deadzone range for joystick centering

// Initialize stepper motor (28BYJ-48) with ULN2003 control pins
Stepper stepper(2048, 2, 4, 3, 5);  // IN1, IN2, IN3, IN4

// Headlight state tracking
bool headlightsOn = false;
bool lastButtonState = HIGH;  // Initial button state

void setup() {
  pinMode(motorPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);  // Use internal pull-up for button
  stepper.setSpeed(10);              // RPM of stepper
  Serial.begin(9600);                // Serial Monitor for debugging
}

void loop() {
  // --- Read Inputs ---
  int xVal = analogRead(A0);  // Steering (X-axis)
  int yVal = analogRead(A1);  // Speed control (Y-axis)
  bool buttonState = digitalRead(buttonPin);

  // --- Toggle Headlights on Button Press ---
  if (lastButtonState == HIGH && buttonState == LOW) {
    headlightsOn = !headlightsOn;
    digitalWrite(ledPin, headlightsOn);
    Serial.println(headlightsOn ? "Headlights ON" : "Headlights OFF");
    delay(200);  // Debounce delay
  }
  lastButtonState = buttonState;

  // --- DC Motor Control ---
  if (abs(yVal - 512) > DEADZONE) {
    int motorSpeed = map(abs(yVal - 512), 0, 512, 0, 255);  // PWM based on joystick distance
    analogWrite(motorPin, motorSpeed);
    Serial.print("Motor Speed: ");
    Serial.println(motorSpeed);
  } else {
    analogWrite(motorPin, 0);
    Serial.println("Motor Stopped");
  }

  // --- Stepper Motor Control (Steering) ---
  if (xVal < 512 - DEADZONE) {
    stepper.step(-30);  // Turn left
    Serial.println("Steering: LEFT");
  } else if (xVal > 512 + DEADZONE) {
    stepper.step(30);  // Turn right
    Serial.println("Steering: RIGHT");
  }

  delay(100);  // Input smoothing
}
