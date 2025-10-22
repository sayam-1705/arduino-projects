// -------------------------------------------
// Code to Test PS2 Joystick Module
// Reads analog values from the joystick's X and Y axes, and also checks the button state (SW pin)

const int joystickX = A0;  // Joystick X-axis connected to A0
const int joystickY = A1;  // Joystick Y-axis connected to A1
const int buttonPin = 8;   // Joystick button connected to pin 8

void setup() {
  Serial.begin(9600);                // Initialize serial communication for debugging
  pinMode(buttonPin, INPUT_PULLUP);  // Joystick button pin set to input with internal pull-up resistor
  Serial.println("PS2 Joystick Test Initialized...");
}

void loop() {
  int xVal = analogRead(joystickX);           // Read X-axis value from joystick
  int yVal = analogRead(joystickY);           // Read Y-axis value from joystick
  bool buttonState = digitalRead(buttonPin);  // Read the button state (pressed or not)

  Serial.print("X: ");  // Print X-axis value
  Serial.print(xVal);
  Serial.print(" | Y: ");  // Print Y-axis value
  Serial.print(yVal);

  // Print button status
  Serial.print(" | Button: ");
  if (buttonState == LOW) {
    Serial.println("Pressed");
  } else {
    Serial.println("Released");
  }

  delay(200);  // Delay for readability in serial output
}
