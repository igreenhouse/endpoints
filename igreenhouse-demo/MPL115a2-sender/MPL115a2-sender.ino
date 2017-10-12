// Feather9x_RX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (receiver)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_TX

// For MPL115a2:
// https://www.adafruit.com/product/992

// Using the sensor is easy. For example, if you're using an Arduino,
// simply connect the VIN pin to the 5V voltage pin, GND to ground,
// SCL to I2C Clock (Analog 5 on an UNO) and SDA to I2C Data (Analog 4 on an UNO).

// On Feather:
// I2C clock is on pin 3
// I2C data is on pin 2
 
#include <SPI.h>
#include <RH_RF95.h>
#include <time.h>
//dependances pour capteur I2C MPL115A2
#include <Wire.h>
#include <Adafruit_MPL115A2.h>
/* for feather32u4 */
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7
 
// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 868.0
 
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
 
// Blinky on receipt
#define LED 13

Adafruit_MPL115A2 mpl115a2;

unsigned char numero = 0;
float pressureKPA = 0, temperatureC = 0;

void setup() 
{
  pinMode(LED, OUTPUT);     
  pinMode(RFM95_RST, OUTPUT);  
  digitalWrite(RFM95_RST, HIGH);
 
  //while (!Serial);
  Serial.begin(9600);
  delay(100);
 
  Serial.println("Feather LoRa RX Test!");
  
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
 
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");
 
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
 
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
 
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
  srand(time(NULL));
    
  Serial.println("Getting barometric pressure from Adafruit_MPL115A2 ...");
  mpl115a2.begin();
}
 
void loop()
{
  uint16_t fi[3];
  uint8_t *buf = (uint8_t *)fi;
  
  digitalWrite(LED, HIGH);
  delay(10);
      
  // Send 
  mpl115a2.getPT(&pressureKPA,&temperatureC);
  
  fi[0]=numero++;
  fi[1] = (uint16_t)(temperatureC * 100);
  fi[2] = (uint16_t)(pressureKPA * 100);
  
  rf95.send(buf, 3*sizeof(uint16_t));
  rf95.waitPacketSent();
  
  Serial.print("Temperature MPL115A2 : ");
  Serial.print(temperatureC);
  Serial.print(" , Pression atmospherique : ");
  Serial.print(pressureKPA);
  Serial.print("\n");
  digitalWrite(LED, LOW);
  delay(10000);
}
