#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

TinyGPSPlus gps;
SoftwareSerial SerialGPS(4, 5);

const char* ssid = "Udeesha";
const char* password = "12345678";

String deviceId = "FF5733";
String driverName = "Lewis Hamilton";
String url = "https://ipc-node-coursework.onrender.com/reading";

float latitude = 0, longitude = 0;
String latitudeString  = "", longitudeString = "";

WiFiServer server(80);
void setup()
{
  Serial.begin(9600);
  SerialGPS.begin(9600);
  Serial.println();
  Serial.println("Connecting");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop()
{
  while (SerialGPS.available() > 0) {
    if (gps.encode(SerialGPS.read()))
    {
      readGPSData();
    }
  }
}
bool setHTTPRequest()
{
  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient https;


  String requestParams = "deviceId=" + deviceId + "&longitude=" + longitudeString + "&latitude=" + latitudeString + "&driverName=" + driverName;


  if (https.begin(client, url)) {
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpCode = https.POST(requestParams);
    Serial.println("Response code: " + String(httpCode));
    if (httpCode > 0) {
      Serial.println(https.getString());
    }
    https.end();
    return true;
  }
  else {
    Serial.printf("Connection failed\n");
    return false;
  }
}

bool getLatLng() {
  if (gps.location.isValid())
  {
    latitude = gps.location.lat();
    latitudeString = String(latitude , 6);
    longitude = gps.location.lng();
    longitudeString = String(longitude , 6);
    return true;
  }
  else {
    return false;
  }
}

void readGPSData()
{
  bool isLatLng  = getLatLng();

  if (isLatLng == true)
  {
    setHTTPRequest();
    delay(10000);
  }
}
