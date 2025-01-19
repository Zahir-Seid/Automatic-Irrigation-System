int rain_status = 0; // Start with 0 (No Rain)

void setup() {
  // Initialize Serial communication for debugging
  Serial.begin(9600);

  // Configure LED_BUILTIN as OUTPUT (predefined for the ESP8266)
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // Output the current rain status on the built-in LED pin
  digitalWrite(LED_BUILTIN, rain_status);

  // Print the status to Serial for debugging
  Serial.print("Rain Status (Mock): ");
  Serial.println(rain_status == 1 ? "Rain Detected" : "No Rain");

  // Toggle the rain status for the next cycle
  rain_status = !rain_status; // Flip between 0 and 1

  // Wait for 1 second before changing the status (adjust as needed)
  delay(1000); // 1 second for testing (can be set to 1 minute)
}
