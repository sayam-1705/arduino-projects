const int trigPin = 9;    // Trig pin on HC-SR04
const int echoPin = 8;    // Echo pin on HC-SR04
const int ledLow = 3;     // LED for low level
const int ledMedium = 4;  // LED for medium level
const int ledHigh = 5;    // LED for high level
const int ledFull = 6;    // LED for full level
const int buzzerPin = 7;  // Buzzer pin

// Define tank height (distance from sensor to bottom of the tank)
const int tankHeight = 30;  // in centimeters, adjust based on actual tank height

void setup() {
  Serial.begin(9600);        // Initialize serial communication
  pinMode(trigPin, OUTPUT);  // Set trigPin as output
  pinMode(echoPin, INPUT);   // Set echoPin as input

  // Set LEDs and buzzer as outputs
  pinMode(ledLow, OUTPUT);
  pinMode(ledMedium, OUTPUT);
  pinMode(ledHigh, OUTPUT);
  pinMode(ledFull, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  // Measure distance to water level
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;  // Convert time to distance in cm

  // Calculate the water level percentage
  int waterLevel = tankHeight - distance;
  int levelPercentage = (waterLevel * 100) / tankHeight;

  Serial.print("Water Level: ");
  Serial.print(levelPercentage);
  Serial.println("%");

  // Control LEDs based on level percentage
  if (levelPercentage < 25) {
    digitalWrite(ledLow, HIGH);
    digitalWrite(ledMedium, LOW);
    digitalWrite(ledHigh, LOW);
    digitalWrite(ledFull, LOW);
  } else if (levelPercentage < 50) {
    digitalWrite(ledLow, HIGH);
    digitalWrite(ledMedium, HIGH);
    digitalWrite(ledHigh, LOW);
    digitalWrite(ledFull, LOW);
  } else if (levelPercentage < 75) {
    digitalWrite(ledLow, HIGH);
    digitalWrite(ledMedium, HIGH);
    digitalWrite(ledHigh, HIGH);
    digitalWrite(ledFull, LOW);
  } else if (levelPercentage >= 75) {
    digitalWrite(ledLow, HIGH);
    digitalWrite(ledMedium, HIGH);
    digitalWrite(ledHigh, HIGH);
    digitalWrite(ledFull, HIGH);

    // Sound buzzer if water level is 100%
    if (levelPercentage >= 100) {
      digitalWrite(buzzerPin, HIGH);
    } else {
      digitalWrite(buzzerPin, LOW);
    }
  }

  delay(1000);  // Wait a second before taking another reading
}
