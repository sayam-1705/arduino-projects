// -------------------------------------------
// Code to Test DC Motor (via 2N2222 transistor)
// This code toggles the DC motor ON and OFF every 2 seconds

const int motorPin = 9;  // Arduino pin connected to the base of 2N2222 via a current-limiting resistor

void setup() {
  Serial.begin(9600);         // Start serial communication for debugging
  pinMode(motorPin, OUTPUT);  // Set motor control pin as output
  Serial.println("DC Motor Test Initialized...");
}

void loop() {
  Serial.println("Motor ON");
  digitalWrite(motorPin, HIGH);  // Turn motor ON
  delay(2000);                   // Run for 2 seconds

  Serial.println("Motor OFF");
  digitalWrite(motorPin, LOW);  // Turn motor OFF
  delay(2000);                  // Wait for 2 seconds
}
