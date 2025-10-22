const int sensorPin = 7;  // Pin connected to the OUT pin of the IR sensor
int sensorValue;          // Variable to store sensor state

void setup() {
  Serial.begin(9600);         // Initialize serial communication at 9600 baud rate
  pinMode(sensorPin, INPUT);  // Set the sensor pin as input
}

void loop() {
  sensorValue = digitalRead(sensorPin);  // Read the sensor's output

  if (sensorValue == HIGH) {
    Serial.println("Object Detected");  // Print message if object is detected
  } else {
    Serial.println("No Object");  // Print message if no object is detected
  }

  delay(500);  // Wait half a second before checking again
}
