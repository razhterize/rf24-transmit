#include <Arduino.h>
#include <SPI.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "printf.h"
#include "RF24.h"

float conc(int a, float b);
float ph_read();

RF24 tf(7,8);
OneWire sense(A0);
DallasTemperature DS18B20(&sense);
uint8_t addresses[][6] = {"Node1", "Node2"};
// float data1, data2;
// float sent1, sent2;

float data1;
float sent1;

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
  data1 = DS18B20.getTempCByIndex(0);
  // data2 = ph_read();
  sent1 = conc(4,data1);
  // sent2 = conc(2,data2);
  if (millis() - prevMillis >= 500){
    prevMillis = millis();
    bool result1 = tf.write(&sent1, sizeof(sent1));
    // bool result2 = tf.write(&sent2, sizeof(sent2));
    Serial.println(data1);
    // Serial.println(data2);
    if (result1){
      Serial.print(F("Transmission (temp) successful! "));          // payload was delivered
      Serial.print(F("Sent: \n"));                               // print payload sent
      Serial.println(sent1);
      sent1 = 0;
    }
    // if (result2){
    //   Serial.print(F("Transmission (pH) successful! "));          // payload was delivered
    //   Serial.print(F("Sent: \n"));                               // print payload sent
    //   Serial.println(sent2);
    //   sent2 = 0;
    // }
    if (!result1){
      Serial.println("Transmission Failed");
    }
  }  
}

float conc(int a, float b){
  String sub1 = String(a);
  String sub2 = String(b);

  String all = sub1 + sub2;
  float val = all.toFloat();
  return val;
}

// float ph_read(){
//   float ph7 = 2.470;
//   float ph4 = 3.020;
//   int raw = analogRead(A1);
//   float volt = raw * 5/1024-0.1;
//   float step = (ph4 - ph7) / 3;
//   float value = 7.0 + ((ph7 - volt)/step);
//   return value;
// }