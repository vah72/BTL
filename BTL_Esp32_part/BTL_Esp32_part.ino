#include "ThingSpeak.h"
#include <WiFi.h>
#include "SPIFFS.h"
#include "secrets.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
WiFiClient client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char* myWriteAPIKey = SECRET_WRITE_APIKEY;

WiFiServer server(80);

float temperature, pressure, windSpeed, rain, humidity;
int direction;

void setup() {
  Serial.begin(115200);  //Initialize serial
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for Leonardo native USB port only
  }

  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
    //connect to wifi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
    Serial.println(WiFi.localIP());
    server.begin();
  }
}

void loop() {

  //nhiet do, ap suat, toc do gio, luong mua, huong gio, do am
  String data = Serial.readStringUntil('\n');
  Serial.println(data);
  if (data != "") {
    int index1 = data.indexOf(',');
    int index2 = data.indexOf(',', index1 + 1);
    int index3 = data.indexOf(',', index2 + 1);
    int index4 = data.indexOf(',', index3 + 1);
    int index5 = data.indexOf(',', index4 + 1);
    float temperature = data.substring(0, index1).toFloat();
    float pressure = data.substring(index1 + 1, index2).toFloat();
    float windSpeed = data.substring(index2 + 1, index3).toFloat();
    float rain = data.substring(index3 + 1, index4).toFloat();
    int direction = data.substring(index4 + 1, index5).toInt();
    float humidity = data.substring(index5 + 1).toFloat();
    Serial.println(temperature);
    Serial.println(pressure);
    Serial.println(windSpeed);
    Serial.println(rain);
    Serial.println(direction);
    Serial.println(humidity);

    ThingSpeak.setField(1, temperature);
    ThingSpeak.setField(2, pressure);
    ThingSpeak.setField(3, windSpeed);
    ThingSpeak.setField(4, rain);
    ThingSpeak.setField(5, direction);
    ThingSpeak.setField(6, humidity);
    int status = ThingSpeak.writeFields(6, myWriteAPIKey);
    if (status == 200) {
      Serial.println("Data sent successfully");
    } else {
      Serial.println("Error sending data");
    }
  } else Serial.println("No data");
  delay(5000);  //wait 5 seconds
}
