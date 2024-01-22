#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Pin assignments
const int fanPin = 9;           // Connect the fan to digital pin 9
const int ledPin = 13;          // Built-in LED pin
const int I2C_ADDR = 0x27;      // I2C address for the LCD module
const int LCD_COLS = 16;        // Number of columns in the LCD
const int LCD_ROWS = 2;         // Number of rows in the LCD
const int dhtPin = 2;           // Connect the DHT11 sensor to digital pin 2

// Constants
const int thresholdTemp = 30;   // Temperature threshold to turn on the LED
const int maxFanSpeed = 255;    // Maximum fan speed

// Variables
float currentTemp = 0;          // Current temperature read by the sensor
int fanSpeed = 0;               // Fan speed based on temperature

// Initialize the LCD module
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLS, LCD_ROWS);

// Initialize the DHT sensor
DHT dht(dhtPin, DHT11);

void setup() {
  // Set up the LCD module
  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.backlight();
  
  // Set fanPin as an output
  pinMode(fanPin, OUTPUT);
  
  // Set ledPin as an output
  pinMode(ledPin, OUTPUT);
  
  // Initialize the DHT sensor
  dht.begin();
}

void loop() {
  // Read temperature from the DHT11 sensor
  currentTemp = dht.readTemperature();
  
  // Gradually adjust the fan speed based on temperature
  if (currentTemp <= thresholdTemp) {
    fanSpeed = map(currentTemp, 0, thresholdTemp, 0, maxFanSpeed);
  } else {
    fanSpeed = maxFanSpeed; // Set fan speed to maximum if temperature exceeds threshold
  }
  
  // Turn on the LED if temperature exceeds the threshold
  if (currentTemp > thresholdTemp) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
  
  // Display temperature and fan speed on the LCD
  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print("Temp: ");
  lcd.print(currentTemp);
  lcd.print("C");
  
  lcd.setCursor(0, 1);
  lcd.print("Speed: ");
  lcd.print(fanSpeed);
  
  // Set the fan speed
  analogWrite(fanPin, fanSpeed);
  
  delay(100); // Delay for 1 second before the next iteration
}
