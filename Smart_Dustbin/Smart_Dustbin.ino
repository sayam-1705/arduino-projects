#include <Servo.h>

// Define pins
const int pirPin = 2;            // PIR sensor output pin
const int servoPin = 9;          // Servo motor signal pin
const int trigPin = 10;          // Ultrasonic sensor trigger pin
const int echoPin = 11;          // Ultrasonic sensor echo pin
const int buzzerPin = 3;         // Buzzer pin

Servo trashLidServo;             // Create servo object

// Time variables
unsigned long lidOpenTime = 3000;  // Duration to keep the lid open (in milliseconds)
unsigned long lastMotionTime = 0;  // Time of the last detected motion
unsigned long warningTime = 2500;  // Time to start the warning buzzer before lid closes

// State variable
bool lidIsOpen = false;

void setup() {
  pinMode(pirPin, INPUT);            // Set PIR pin as input
  pinMode(trigPin, OUTPUT);          // Set Ultrasonic trig pin as output
  pinMode(echoPin, INPUT);           // Set Ultrasonic echo pin as input
  pinMode(buzzerPin, OUTPUT);        // Set buzzer pin as output
  
  trashLidServo.attach(servoPin);    // Attach servo to pin
  trashLidServo.write(0);            // Initial position for the servo (closed position)

  Serial.begin(9600);                // Initialize Serial Monitor for debugging
  Serial.println("System Initialized. Waiting for motion...");
}

void loop() {
  // Measure distance using ultrasonic sensor
  long distance = measureDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Only activate PIR if distance is less than 30 cm
  if (distance < 30) {
    int pirState = digitalRead(pirPin);  // Read PIR sensor state

    // If motion is detected and lid is closed
    if (pirState == HIGH && !lidIsOpen) {
      Serial.println("Motion detected! Opening lid...");
      
      // Record the time of motion detection
      lastMotionTime = millis();
      
      // Open the lid (set servo to 90°)
      trashLidServo.write(90);
      lidIsOpen = true;
    }

    // Check if it's time to start the warning buzzer and close the lid
    if (lidIsOpen && (millis() - lastMotionTime > warningTime)) {
      // Start the buzzer warning
      digitalWrite(buzzerPin, HIGH);
      delay(500);                  // Buzzer on for 500ms
      digitalWrite(buzzerPin, LOW); // Turn buzzer off

      // Close the lid after the full lid open time
      if (millis() - lastMotionTime > lidOpenTime) {
        trashLidServo.write(0);    // Close the lid (set servo to 0°)
        Serial.println("No motion detected. Closing lid...");
        lidIsOpen = false;
      }
    }
  }

  delay(100);  // Short delay to reduce false triggers
}

// Function to measure distance using ultrasonic sensor
long measureDistance() {
  // Send a 10µs pulse to trigger the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Measure the time it takes for the echo to return
  long duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance in centimeters
  long distance = duration * 0.034 / 2;
  return distance;
}
