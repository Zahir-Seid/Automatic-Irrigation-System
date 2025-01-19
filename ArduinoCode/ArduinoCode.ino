#include <LiquidCrystal.h>
#include "DHT.h"

// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

// Pin definitions
const int relay_Pin = 8;       // Relay control pin
const int DHT11_Sensor = 9;    // DHT11 sensor pin
const int moisture_sensor = A0; // Soil moisture sensor analog pin
const int rain_Sensor = 10;    // Rain signal pin (connected to ESP8266)

#define DHTTYPE DHT11
int moisture_sensor_value;
int rain_status; // Holds the rain status (0 = No Rain, 1 = Rain Detected)
float humidity_value, temperature_value;
DHT dht(DHT11_Sensor, DHTTYPE);

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);

  // Configure pins
  pinMode(relay_Pin, OUTPUT);
  pinMode(rain_Sensor, INPUT);
  lcd.begin(16, 2);

  // Display startup message
  lcd.print("Smart Irrigation");
  lcd.setCursor(0, 1);
  lcd.print("SYSTEM");

  // Initialize DHT sensor
  dht.begin();

  // Set relay to OFF initially
  digitalWrite(relay_Pin, LOW);

  delay(2000);
}

void loop() {
  // Read sensors and rain status
  readDHT11Sensor();
  moisture_level_detected();
  readRainStatus();

  // Start or stop the motor based on conditions
  water_motor_start();

  delay(1000); // Add some delay for stability
}

void readDHT11Sensor() {
  // Read humidity and temperature
  humidity_value = dht.readHumidity();
  temperature_value = dht.readTemperature();

  // Check if sensor readings are valid
  if (isnan(humidity_value) || isnan(temperature_value)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print and display humidity
  Serial.print("Humidity: ");
  Serial.print(humidity_value);
  Serial.println("%");
  lcd.clear();
  lcd.print("Humidity %: ");
  lcd.setCursor(0, 1);
  lcd.print(humidity_value);

  delay(1000);

  // Print and display temperature
  Serial.print("Temperature: ");
  Serial.print(temperature_value);
  Serial.println("C");
  lcd.clear();
  lcd.print("Temperature C:");
  lcd.setCursor(0, 1);
  lcd.print(temperature_value);

  delay(1000);
}

void moisture_level_detected() {
  // Read soil moisture sensor value
  moisture_sensor_value = analogRead(moisture_sensor);

  // Print and display moisture level
  Serial.print("Moisture Level: ");
  Serial.println(moisture_sensor_value);
  lcd.clear();
  lcd.print("Moisture Level:");
  lcd.setCursor(0, 1);
  lcd.print(moisture_sensor_value);

  delay(1000);
}

void readRainStatus() {
  // Read rain status from pin 10 (connected to ESP8266)
  rain_status = digitalRead(rain_Sensor); // Returns 1 for HIGH, 0 for LOW

  // Print rain status for debugging
  Serial.print("Rain Status: ");
  Serial.println(rain_status == 1 ? "Rain Detected" : "No Rain");
}

void water_motor_start() {
  // If rain is detected (rain_status = 1)
  if (rain_status == 1) {
    digitalWrite(relay_Pin, 0); // Turn off the water pump
    lcd.clear();
    lcd.print("Rain Detected");
    lcd.setCursor(0, 1);
    lcd.print("Motor OFF");
    delay(1000);
  }
  // If no rain and moisture level is low
  else {
    if (moisture_sensor_value > 700) {
      digitalWrite(relay_Pin, 1); // Turn on the water pump
      lcd.clear();
      lcd.print("Low water level");
      lcd.setCursor(0, 1);
      lcd.print("Motor ON");
      delay(1000);
    } else {
      digitalWrite(relay_Pin, 0); // Turn off the water pump
      lcd.clear();
      lcd.print("Water Level Ok");
      lcd.setCursor(0, 1);
      lcd.print("Motor OFF");
      delay(1000);
    }
  }
}

