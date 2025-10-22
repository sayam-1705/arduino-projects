// Blynk Configuration
#define BLYNK_TEMPLATE_ID "TMPL3rwiCl-ed"
#define BLYNK_TEMPLATE_NAME "Sustainable Agriculture"
#define BLYNK_AUTH_TOKEN "ofu5OWsDV8g_STdu8gMJvLNAWXN86bCF"

// WiFi Credentials
#define WIFI_SSID "Galaxy S22 E758"
#define WIFI_PASS "8august05"

// Include Libraries
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"

// Define Sensor, Relay, LED, and Buzzer Pins
#define SOIL_MOISTURE_PIN 34  // Analog Pin for Soil Moisture Sensor
#define DHTPIN 4              // Digital Pin for DHT Sensor
#define RELAY_PIN 26          // Digital Pin for Relay (Water Pump)
#define LED_PIN 27            // Digital Pin for LED (Pump Status)
#define BUZZER_PIN 25         // Digital Pin for Buzzer (Pump Status)
#define DHTTYPE DHT11         // Define DHT Sensor Type (DHT11)

// Create Sensor & Timer Instances
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// WiFi & Blynk Auth Variables
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = WIFI_SSID;
char pass[] = WIFI_PASS;

// Default Crop Selection & Moisture Thresholds
String crop = "Rice";
int moistureThreshold;

// Crop-Specific Moisture Levels (Percentage)
int riceThreshold = 70;
int wheatThreshold = 71;
int tomatoThreshold = 72;
int potatoThreshold = 70;

// Define Ideal Temperature & Humidity Ranges for Crops
struct CropConditions {
    int minTemp, maxTemp;
    int minHumidity, maxHumidity;
};

// Define Conditions for Different Crops
CropConditions rice = {20, 35, 60, 80};
CropConditions wheat = {10, 25, 50, 70};
CropConditions tomato = {18, 30, 50, 80};
CropConditions potato = {10, 22, 60, 80};

// Store Current Crop Conditions
CropConditions currentConditions;

// Function to Set Crop Thresholds & Ideal Conditions
void setCropThreshold(String selectedCrop) {
    if (selectedCrop == "Rice") {
        moistureThreshold = riceThreshold;
        currentConditions = rice;
    } else if (selectedCrop == "Wheat") {
        moistureThreshold = wheatThreshold;
        currentConditions = wheat;
    } else if (selectedCrop == "Tomato") {
        moistureThreshold = tomatoThreshold;
        currentConditions = tomato;
    } else if (selectedCrop == "Potato") {
        moistureThreshold = potatoThreshold;
        currentConditions = potato;
    } else {
        Serial.println("Invalid Crop! Defaulting to Rice.");
        moistureThreshold = riceThreshold;
        currentConditions = rice;
    }

    Serial.print("Selected Crop: ");
    Serial.print(selectedCrop);
    Serial.print(", Moisture Threshold: ");
    Serial.println(moistureThreshold);
}

// Function to Provide Climate-Based Advice
String getClimateAdvice(float temp, float hum) {
    if (temp < currentConditions.minTemp) {
        return "Temperature too low! Consider insulation.";
    } else if (temp > currentConditions.maxTemp) {
        return "Temperature too high! Provide shade/watering.";
    } else if (hum < currentConditions.minHumidity) {
        return "Humidity too low! Use mulching or misting.";
    } else if (hum > currentConditions.maxHumidity) {
        return "Humidity too high! Improve ventilation.";
    } else {
        return "Conditions are optimal.";
    }
}

// Function to Check Soil Moisture & Control Irrigation
void checkSoilMoisture() {
    int moisture = analogRead(SOIL_MOISTURE_PIN);
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    // Convert Moisture Sensor Value to Percentage (0-100%)
    moisture = map(moisture, 4095, 0, 0, 100);  // Invert mapping

    Serial.print("Soil Moisture: ");
    Serial.print(moisture);
    Serial.print("%, Temperature: ");
    Serial.print(temp);
    Serial.print("C, Humidity: ");
    Serial.println(hum);

    // Send Data to Blynk App
    Blynk.virtualWrite(V1, moisture);
    Blynk.virtualWrite(V2, temp);
    Blynk.virtualWrite(V3, hum);
    Blynk.virtualWrite(V4, crop);

    // Climate Advice
    String advice = getClimateAdvice(temp, hum);
    Blynk.virtualWrite(V6, advice);
    Serial.println(advice);

    // Automatic Irrigation Control
    if (moisture < moistureThreshold) {
        digitalWrite(RELAY_PIN, LOW);  // Turn ON Pump
        digitalWrite(LED_PIN, HIGH);   // Turn ON LED
        digitalWrite(BUZZER_PIN, HIGH); // Turn ON Buzzer
        Blynk.virtualWrite(V5, "PUMP ON");
    } else {
        digitalWrite(RELAY_PIN, HIGH); // Turn OFF Pump
        digitalWrite(LED_PIN, LOW);    // Turn OFF LED
        digitalWrite(BUZZER_PIN, LOW); // Turn OFF Buzzer
        Blynk.virtualWrite(V5, "PUMP OFF");
    }
}

// Function to Change Crop via Blynk App
BLYNK_WRITE(V7) {
    crop = param.asStr();
    setCropThreshold(crop);
}

// Function for Manual Pump Control from Blynk
BLYNK_WRITE(V8) {
    int buttonState = param.asInt();
    digitalWrite(RELAY_PIN, buttonState ? LOW : HIGH);
    digitalWrite(LED_PIN, buttonState ? HIGH : LOW);
    digitalWrite(BUZZER_PIN, buttonState ? HIGH : LOW);
}

// Setup Function
void setup() {
    Serial.begin(115200);
    pinMode(SOIL_MOISTURE_PIN, INPUT);
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    digitalWrite(RELAY_PIN, HIGH);  // Keep Pump OFF Initially
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);

    dht.begin();

    // Connect to WiFi
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("✅ Connected to WiFi!");

    // Print ESP32 IP Address
    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.localIP());

    // Test Blynk Server Connection
    WiFiClient client;
    if (!client.connect("blynk.cloud", 80)) {
        Serial.println("⚠ Unable to reach Blynk server!");
    } else {
        Serial.println("✅ Blynk server reachable!");
        client.stop();
    }

    // Manually Configure Blynk
    Blynk.config(auth);
    Blynk.connect();

    if (Blynk.connected()) {
        Serial.println("✅ Connected to Blynk!");
    } else {
        Serial.println("❌ Failed to connect to Blynk!");
    }

    // Set Default Crop Threshold
    setCropThreshold(crop);

    // Run checkSoilMoisture() every 5 seconds
    timer.setInterval(5000L, checkSoilMoisture);
}

// Loop Function
void loop() {
    if (Blynk.connected()) {
        Blynk.run();
    } else {
        Serial.println("⚠ Blynk is not connected, retrying...");
        Blynk.connect();
    }
    timer.run();
}
