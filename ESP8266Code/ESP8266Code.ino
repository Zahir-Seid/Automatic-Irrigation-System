#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// Wi-Fi credentials
const char* ssid = "YourWiFiSSID";        // Replace with your Wi-Fi SSID
const char* password = "YourWiFiPassword"; // Replace with your Wi-Fi Password

// OpenWeatherMap API details
const String apiKey = "API_Key"; // Replace with your API key
const String location = "Addis Ababa,ET";                // Replace with your location
const String units = "metric";                           // Use "imperial" for Fahrenheit, "metric" for Celsius

void setup() {
  Serial.begin(9600);
  Serial.println("ESP8266 Initialized");

  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Connected!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    // OpenWeatherMap 5-day/3-hour forecast API request
    String forecastURL = "http://api.openweathermap.org/data/2.5/forecast?q=" + location + "&appid=" + apiKey + "&units=" + units;
    http.begin(client, forecastURL);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Forecast Data Received: " + response);

      // Parse JSON
      DynamicJsonDocument doc(4096);
      deserializeJson(doc, response);

      bool rainExpected = false;

      // Loop through the forecast for the next 24 hours (8 intervals)
      for (int i = 0; i < 8; i++) {
        String condition = doc["list"][i]["weather"][0]["main"];
        if (condition == "Rain") {
          rainExpected = true;
          break;
        }
      }

      // Print and send rain status
      Serial.println("Rain Expected Today: " + String(rainExpected ? "Yes" : "No"));
      Serial.write(rainExpected ? '1' : '0');
    } else {
      Serial.println("Error fetching forecast data!");
    }

    http.end();
  } else {
    Serial.println("Wi-Fi Disconnected!");
  }

  delay(3600000); // Check once every hour (3600000 ms)
}
