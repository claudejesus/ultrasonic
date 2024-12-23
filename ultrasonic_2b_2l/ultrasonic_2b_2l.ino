#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Initialize LCD
LiquidCrystal_I2C lcd(0x20, 16, 2);

// Define pin connections
#define ENTRY_BUTTON 8
#define EXIT_BUTTON 9
#define GREEN_LED 3
#define RED_LED 2
#define TRIG_PIN 6
#define ECHO_PIN 5

// Parking spaces and variables
int totalSpaces = 10; 
int freeSpaces = 10;  

void setup() {
  pinMode(ENTRY_BUTTON, INPUT_PULLUP);
  pinMode(EXIT_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Green LED ON initially
  digitalWrite(GREEN_LED, HIGH); 
  digitalWrite(RED_LED, LOW);   

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Welcome message
  lcd.setCursor(0, 0);
  lcd.print("Welcome to Goico");
  Serial.begin(9600);
  Serial.write("Welcome to Goico");
  Serial.write("Parking System\n");
  lcd.setCursor(0, 1);
  lcd.print("Parking System");
  Serial.write("Parking System\n");

  delay(2000); // Wait for 2 seconds to show the welcome message
  
  // Initial free spaces display
  updateDisplay();
}

void loop() {
  // Entry button logic
  if (digitalRead(ENTRY_BUTTON) == LOW) {
    delay(200); // Debouncing
    if (freeSpaces > 0) {
      freeSpaces--;
      updateStatus(); 
    }
  }

  // Exit button logic
  if (digitalRead(EXIT_BUTTON) == LOW) {
    delay(200); // Debouncing
    if (freeSpaces < totalSpaces) {
      freeSpaces++;
      updateStatus();
    }
  }

  // Check the parking status using the ultrasonic sensor
  int distance = measureDistance();
  if (distance < 400 && freeSpaces > 0) { // Assuming <10 cm means a car is detected
    freeSpaces--;
    updateStatus();
    Serial.println(distance);
    delay(1000); // Wait for the car to park
  }
}

// Function to measure distance using the ultrasonic sensor
int measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2; // Convert to cm
  return distance;
  Serial.write(distance);
}

// Function to update LEDs based on free spaces
void updateLEDs() {
  if (freeSpaces == 0) {
    digitalWrite(GREEN_LED, LOW);  // No space available
    digitalWrite(RED_LED, HIGH);  // Red LED ON
  } else {
    digitalWrite(GREEN_LED, HIGH); // Space available
    digitalWrite(RED_LED, LOW);   // Red LED OFF
  }
}

// Function to update the LCD display
void updateDisplay() {
  lcd.clear(); 
  lcd.setCursor(0, 0);
  lcd.print("Spaces Avail: ");
  Serial.write("Spaces Avail: ");
  Serial.println(freeSpaces);
  lcd.print(freeSpaces);
  
  // Show parking status
  lcd.setCursor(0, 1);
  if (freeSpaces == 0) {
    lcd.print("Status: FULL");
    Serial.write("Status: FULL\n");
  } else {
    lcd.print("Status: OPEN");
    Serial.write("Status: OPEN\n");
  }
}

// Function to update both LEDs and LCD
void updateStatus() {
  updateLEDs();    
  updateDisplay();
}
