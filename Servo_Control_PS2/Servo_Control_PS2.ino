#include <Servo.h>

Servo myServo;  // Create a Servo object

const int joyX = A0;     // Joystick X-axis
const int joyY = A1;     // Joystick Y-axis (not used in this project)
const int servoPin = 9;  // Servo signal pin

void setup() {
  myServo.attach(servoPin);  // Attach the servo
  pinMode(joyX, INPUT);
  Serial.begin(9600);
}

void loop() {
  int xValue = analogRead(joyX);                  // Read joystick X-axis value
  int servoAngle = map(xValue, 0, 1023, 0, 180);  // Convert to servo angle (0-180°)

  myServo.write(servoAngle);  // Move servo
  Serial.print("X: ");
  Serial.print(xValue);
  Serial.print(" -> Servo Angle: ");
  Serial.println(servoAngle);

  delay(10);
}
