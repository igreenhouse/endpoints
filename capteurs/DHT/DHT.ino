// Feather9x_RX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (receiver)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_TX
 
#include <SPI.h>
#include <RH_RF95.h>
#include <time.h>
#include "DHT.h"
 
// Blinky on receipt
#define LED 13
#define DHTPIN A1
#define DHTTYPE DHT11

DHT dht(DHTPIN,DHTTYPE);
void setup() 
{
  pinMode(LED, OUTPUT);     
 
  while (!Serial);
  Serial.begin(9600);
  delay(100);
 
  dht.begin();
}
 
double temperature;
double humidite;

void loop()
{   
    temperature = dht.readTemperature(false);
    humidite = dht.readHumidity();
      
    Serial.print("Temperature : " );
    Serial.print(temperature);
    Serial.print(" , Humidite : ");
    Serial.print(humidite);
    Serial.print("\n");
    digitalWrite(LED, LOW);
    delay(10000);
}
