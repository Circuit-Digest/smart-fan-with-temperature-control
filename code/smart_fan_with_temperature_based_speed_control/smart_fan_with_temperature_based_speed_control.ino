// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Hash.h>
#define DHTPIN 0
#include <ESP_EEPROM.h>
#include"index_html.h"

#define DHTTYPE    DHT11

float t = 0.0;
unsigned long previousMillis = 0;
const long interval = 5000;
const char* ssid = "Fan Speed Controller";
const char* password = "12345678";
const int output_pin = 2;
String sliderValue = "0";
String temp_min, temp_max;
const char* PARAM_INPUT = "value";
const char* Temp_Min = "tempMin";
const char* Temp_Max = "tempMax";
float temp_range;
bool auto_manual_mode = 0; // 1 is auto mode o is manual mode
int tmp_mode, tmp_slider = 0;

DHT dht(DHTPIN, DHTTYPE);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


String processor(const String& var) {
  //Serial.println(var);
  if (var == "SLIDERVALUE") {
    return sliderValue;
  }
  else if (var == "TEMPERATURE") {
    return String(t);
  }
  else if (var == "tempMin") {
    return temp_min;
  }
  else if (var == "tempMax") {
    return temp_max ;
  }
  return String();
}
void main_loop();

void setup() {
  // Serial port for debugging purposes
  pinMode(1, FUNCTION_3);
  pinMode(3, FUNCTION_3);
  pinMode(1, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(1, HIGH);

  Serial.begin(115200);

  EEPROM.begin(30);
  EEPROM.get(0, auto_manual_mode);
  float a, b;
  int c;
  EEPROM.get(4, a);
  EEPROM.get(12, b);
  EEPROM.get(20, c);
  temp_min = String(a);
  temp_max = String(b);
  sliderValue = String(c);
  //  Serial.println("\n");
  //  delay(2000);
  //  Serial.println(auto_manual_mode);
  //  Serial.println(temp_min);
  //  Serial.println(temp_max);
  //  Serial.println(sliderValue);
  main_loop();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password); / Set access point

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue = inputMessage;
      auto_manual_mode = 0; // set to manual mode
      EEPROM.put(0, auto_manual_mode);
      EEPROM.put(20, sliderValue.toInt());
      Serial.println(auto_manual_mode);
      boolean ok2 = EEPROM.commit();
      Serial.println((ok2) ? "Second commit OK" : "Commit failed");
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(t).c_str());
  });


  // Send a GET request to <ESP_IP>/get?inputString=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage;
    // GET inputString value on <ESP_IP>/get?inputString=<inputMessage>
    if (request->hasParam(Temp_Min)) {
      inputMessage = request->getParam(Temp_Min)->value();
      temp_min = inputMessage;
      EEPROM.put(4, temp_min.toFloat());
      EEPROM.commit();
      Serial.println(temp_min);

      //writeFile(SPIFFS, "/inputString.txt", inputMessage.c_str());
    }
    // GET inputInt value on <ESP_IP>/get?inputInt=<inputMessage>
    if (request->hasParam(Temp_Max)) {
      inputMessage = request->getParam(Temp_Max)->value();
      temp_max = inputMessage;
      EEPROM.put(12, temp_max.toFloat());
      EEPROM.commit();
      Serial.println(temp_max);


      // writeFile(SPIFFS, "/inputInt.txt", inputMessage.c_str());
    }
    // GET inputFloat value on <ESP_IP>/get?inputFloat=<inputMessage>
    else {
      inputMessage = "No message sent";
    }
    //Serial.println(inputMessage);
    //request->send(200, "text/text", "<html><body><h3>Data Set to ESP</h3></body></html>");
    auto_manual_mode = 1; // set to auto mode
    EEPROM.put(0, auto_manual_mode);
    Serial.println(auto_manual_mode);
    boolean ok1 = EEPROM.commit();
    Serial.println((ok1) ? "First commit OK" : "Commit failed");
  });

  // Start server
  server.begin();
}

void loop() {

  main_loop();

  pinMode(1, FUNCTION_3);
  pinMode(3, FUNCTION_3);
  pinMode(1, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(1, HIGH);

  if (auto_manual_mode != tmp_mode) {

    if (auto_manual_mode == 1) {

      digitalWrite(1, LOW);
      delay(500);
      digitalWrite(1, HIGH);
      delay(500);
      digitalWrite(1, LOW);
      delay(500);
      digitalWrite(1, HIGH);
      delay(500);
    }



    tmp_mode = auto_manual_mode;
  }

  if (sliderValue.toInt() != tmp_slider)
  {
    digitalWrite(1, LOW);
    delay(500);
    digitalWrite(1, HIGH);

    tmp_slider = sliderValue.toInt();

  }

}

void main_loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you updated the DHT values
    previousMillis = currentMillis;
    // Read temperature as Celsius (the default)
    float temp_tmp = dht.readTemperature();

    t = dht.readTemperature();
  }
  if (auto_manual_mode == 0) // thi is manual mode the value is 0
  {
    int val = map(sliderValue.toInt(), 0, 100, 255, 0);
    analogWrite(output_pin, val);
  }

  if (auto_manual_mode) // auto mode
  {
    temp_range = (temp_max.toFloat() - temp_min.toFloat()) / 10;

    if (temp_max.toFloat() <= t)
    {
      analogWrite(output_pin, 25);
    }

    else if ((temp_min.toFloat() + (temp_range * 9)) <= t)
    {
      analogWrite(output_pin, 50);
    }
    else if ((temp_min.toFloat() + (temp_range * 8)) <= t)
    {
      analogWrite(output_pin, 100);
    }

    else if ((temp_min.toFloat() + (temp_range * 7)) <= t)
    {
      analogWrite(output_pin, 125);
    }

    else if ((temp_min.toFloat() + (temp_range * 6)) <= t)
    {
      analogWrite(output_pin, 150);
    }

    else if ((temp_min.toFloat() + (temp_range * 5)) <= t)
    {
      analogWrite(output_pin, 175);
    }

    else if ((temp_min.toFloat() + (temp_range * 4)) <= t)
    {
      analogWrite(output_pin, 200);
    }

    else if ((temp_min.toFloat() + (temp_range * 3)) <= t)
    {
      analogWrite(output_pin, 225);
    }

    else if ((temp_min.toFloat() + (temp_range * 2)) <= t)
    {
      analogWrite(output_pin, 250);
    }

    else if ((temp_min.toFloat() + (temp_range * 1)) <= t)
    {
      analogWrite(output_pin, 255);
    }
  }
}
