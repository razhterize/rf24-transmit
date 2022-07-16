#include <Arduino.h>
#include <SPI.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "printf.h"
#include "RF24.h"

float conc(int a, float b);

RF24 tf(7,8);
OneWire sense(A0);
DallasTemperature DS18B20(&sense);
uint8_t addresses[][6] = {"Node1", "Node2"};
float data;
float sent;

void setup() {
  Serial.begin(115200);
  while (!Serial){
    }
  while (!tf.begin()){
    Serial.println("RF24 Init failed!");
    delay(500);
  }
  Serial.println("Test Run!");
  tf.setPALevel(RF24_PA_LOW);
  tf.openWritingPipe(addresses[1]);
}

unsigned long prevMillis;
unsigned long currentMillis ;
void loop() {
  DS18B20.requestTemperatures();
  data = DS18B20.getTempCByIndex(0);
  sent = conc(2,data);
  if (millis() - prevMillis >= 500){
    prevMillis = millis();
    unsigned long start_timer = micros(); 
    bool result = tf.write(&sent, sizeof(sent));
    unsigned long end_timer = micros();
    Serial.println(data);
    if (result){
      Serial.print(F("Transmission successful! "));          // payload was delivered
      Serial.print(F("Time to transmit = "));
      Serial.print(end_timer - start_timer);                 // print the timer result
      Serial.print(F(" us. Sent: \n"));                               // print payload sent
      Serial.println(sent);
      sent = 0;
    }
    else {
      Serial.println(F("Transmission failed or timed out")); // payload was not delivered
    }
  }  
}

float conc(int a, float b){
  String sub1 = String(a);
  String sub2 = String(b,2);

  String all = sub1 + sub2;
  float val = all.toFloat();
  return val;
}