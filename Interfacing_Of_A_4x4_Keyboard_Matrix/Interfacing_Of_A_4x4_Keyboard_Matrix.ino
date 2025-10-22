#include <Keypad.h>

// Define the number of rows and columns on the keypad
const byte ROWS = 4;
const byte COLS = 4;

// Define the keymap for the 4x4 keypad
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

// Define the row and column pins on the Arduino
byte rowPins[ROWS] = { 2, 3, 4, 5 };  // Connect to the row pins of the keypad
byte colPins[COLS] = { 6, 7, 8, 9 };  // Connect to the column pins of the keypad

// Create the Keypad object
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);  // Initialize serial communication
}

void loop() {
  char key = keypad.getKey();  // Check if a key is pressed

  if (key) {  // If a key is pressed, it returns a value; otherwise, returns null
    Serial.print("Key Pressed: ");
    Serial.println(key);  // Display the key pressed on the Serial Monitor
  }
}
