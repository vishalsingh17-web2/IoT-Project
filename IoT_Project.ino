#include "DHT.h"
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Wire.h>

// set_up for our DHT11 sensor
#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN,DHTTYPE);

//Here we have calibrated our soil moisture sensor
int dryValue = 1024;
int wetValue = 590;
int friendlyDryValue = 0;
int friendlyWetValue = 100;

// Defining LED pins
#define LED D1
#define LED_2 D2

// Setting params
#define FIREBASE_HOST "iot-project-e30c5-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "EtiyyI6b2mWhC9ZDMsIqfw8HtBIwnIm3OmqsE4Bx"
#define WIFI_SSID "OnePlus 7"
#define WIFI_PASSWORD "IoT_Test"

void setup() {
  Serial.begin(9600);
  dht.begin();

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  pinMode(LED, OUTPUT);
  pinMode(LED_2, OUTPUT);  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
}

  int val;
  val = analogRead(A0);
  Serial.print("Moisture Value: " );
  Serial.println(val);
  delay(100);

  //digitalWrite(LED, HIGH);
  int output_value = map(val, dryValue, wetValue, friendlyDryValue, friendlyWetValue);
  Serial.print("Moist %: ");
  Serial.print(output_value);
  Serial.println("%");
  delay(100);

  if (output_value <= 60){
    Serial.println("Mositure Content is low");
    digitalWrite(LED,HIGH);
  }
  else{
    delay(100);
    digitalWrite(LED,LOW);   
  }
    
  // set value
  Firebase.setFloat("Humidity", h);
  // handle error
  if (Firebase.failed()) {
      Serial.print("Humidity failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  
  // update value
  Firebase.setFloat("Temperature", t);
  // handle error
  if (Firebase.failed()) {
      Serial.print("Temperature failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  // set value
  Firebase.setFloat("Soil_Value", output_value);
  // handle error
  if (Firebase.failed()) {
      Serial.print("Soil_Value");
      Serial.println(Firebase.error());  
      return;
  }

  delay(1000);  

  // get value 
  Serial.print("Humidity: ");
  Serial.println(Firebase.getFloat("Humidity"));

  // get value 
  Serial.print("Temperature: ");
  Serial.println(Firebase.getFloat("Temperature"));
  
  // get value 
  Serial.print("Soil_Value: ");
  Serial.println(Firebase.getFloat("Soil_Value"));
  Serial.print("%");


  //get_value_for_switch
  Serial.print("Motor_Status");
  Serial.println(Firebase.getFloat("Motor_Status"));
  if(Firebase.getFloat("Motor_Status") == 1){
    Serial.println("Motor On");
    digitalWrite(LED_2,HIGH);    
  }
  else{
    Serial.println("Motor Off");
    digitalWrite(LED_2,LOW);
  }


}
