
// https://www.electronicwings.com/esp32/vl53l0x-sensor-interfacing-with-esp32
// Nln works! 
#include <Arduino.h>

#ifdef ARDUINO_ARCH_ESP32

#include <WiFi.h>
#include <WebServer.h>
WebServer server(80);

#elif defined(ARDUINO_ARCH_ESP8266)

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);

#endif

#include "html.h"
#include <Wire.h>
#include <VL53L1X.h>   //Pololu
#include <passwords.h> // our WiFi passwords

VL53L1X sensor;

int measuredDist;
int sensorHeight = 300;               //установочная высота сенсора от пола в cm
int palletHeight;                     //вычисленная высота паллета 


void MainPage() {
  //String _html_page = html_page;              /*Read The HTML Page*/
  server.send(200, "text/html", html_page);  /*Send the code to the web server*/
}

void MeasuredData() {
  String data = "[\"" + String(palletHeight) + "\", \"" + String(sensorHeight) + "\", \"" + String(measuredDist) + "\"]";
  server.send(200, "text/plane", data);
}

void setup(void){
  Serial.begin(9600);                 /*Set the baudrate to 115200*/
  delay(3000);

  //-------------VL53L1X Pololu Lib---------------
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C
  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1);
  }
  sensor.setDistanceMode(VL53L1X::Long);
  sensor.setMeasurementTimingBudget(50000); 
  sensor.startContinuous(100);
  
  //--------------WiFi Connection---------------
  WiFi.mode(WIFI_STA);                  /*Set the WiFi in STA Mode*/
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  delay(1000);                          /*Wait for 1000mS*/
  while(WiFi.waitForConnectResult() != WL_CONNECTED){Serial.print(".");} /*Wait while connecting to WiFi*/
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("Your Local IP address is: ");
  Serial.println(WiFi.localIP());            /*Print the Local IP*/
  
  //---------------WEB page---------------------
  server.on("/", MainPage);                  /*Display the Web/HTML Page*/
  server.on("/readHightData", MeasuredData); /*Display the hight value(cm)*/
  server.begin();                            /*Start Server*/
  delay(1000);                               /*Wait for 1000mS*/
}

void loop(void){
  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  measuredDist =  sensor.read() / 10;
  palletHeight = sensorHeight - measuredDist;
  Serial.println("Pallet h(сm): " + String(palletHeight) + "  Sensor h(сm): " + String(sensorHeight) + "  Distance (сm): " + String(measuredDist));  
  server.handleClient();
  //Serial.println();
  delay(1000);
}
