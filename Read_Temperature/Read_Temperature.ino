/**
 * DHT11 Temperature Reader
 * This sketch reads temperature data from the DHT11 sensor and prints the value to the serial port.
 * It also handles potential error states that might occur during reading.
 *
 * Author: Dhruba Saha
 * Version: 2.1.1
 * License: MIT
 */

#include <DHT11.h>

// Specify the DHT11 data pin as Digital Pin 2 for Arduino.
DHT11 dht11(2);

void setup() {
  // Initialize serial communication to allow debugging and data readout.
  Serial.begin(9600);

  // Uncomment to set a custom delay between sensor readings (in milliseconds).
  // dht11.setDelay(500); // Default is 500ms.
}

void loop() {
  // Attempt to read the temperature value from the DHT11 sensor.
  int temperature = dht11.readTemperature();

  // If the reading is successful, print the temperature value.
  // Otherwise, print an appropriate error message.
  if (temperature >= 0) {  // DHT11 temperature is always non-negative if successful.
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
  } else {
    // Print error message based on the error code.
    Serial.println(DHT11::getErrorString(temperature));
  }

  delay(1000);  // Wait 1 second between readings.
}
