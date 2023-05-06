#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <SD.h>
#include <SPI.h>
#include <FS.h>

const char* ssid = "Pxl";
const char* password = "123412345";
void setup() {
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("Serial communication started");

  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount SPIFFS");
    return;
  }
  Serial.println("SPIFFS mounted");

  String gps_coordinates = "14.146319,79.850388";
  String url = "https://api.weatherapi.com/v1/forecast.json?key=c685be0d28b8490a811100656232102&q=" + gps_coordinates + "&days=9&aqi=no&alerts=no";
  Serial.println("Requesting data from Weather API");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  HTTPClient http;
  WiFiClient client;
  
  http.begin(client, url);
  int httpCode = http.GET();
  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println("Data received from Weather API");

    File file = SPIFFS.open("/data.csv", "w");
    if (file) {
      Serial.println("Writing data to CSV file");
      file.println("Date,Time,Temperature (C),Precipitation (mm),Precipitation(%),Will_it_rain?");
      DynamicJsonDocument doc(4096);
      deserializeJson(doc, payload);
      JsonArray forecasts = doc["forecast"]["forecastday"].as<JsonArray>();
      for (auto forecast : forecasts) {
        String date = forecast["date"].as<String>();
        JsonArray hours = forecast["hour"].as<JsonArray>();
        for (auto hour : hours) {
          String time = hour["time"].as<String>();
          String temperature = hour["temp_c"].as<String>();
          String precipitation = hour["precip_mm"].as<String>();
          String precipitation_percentage = hour["chance_of_rain"].as<String>();
          String will_it_rain = hour["will_it_rain"].as<String>();
          file.println(date + "," + time + "," + temperature + "," + precipitation + "," + precipitation_percentage + "," + will_it_rain);
        }
      }
      file.close();
      Serial.println("Data written to CSV file");
    } else {
      Serial.println("Failed to open CSV file");
    }
  } else {
    Serial.println("Failed to get data from Weather API");
  }

  http.end();
}

void loop() {}
