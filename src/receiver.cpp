#include <Arduino.h>
#include "CAN.h"

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  while (!Serial) {
    delay (50) ;
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
  }
  delay(100);

  Serial.println("CAN RECEIVER init...");
  // start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    Serial.println("CAN init failed");
    while (1);
  }
  Serial.println("CAN init successful");
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:

  // try to parse packet
  int packetSize = CAN.parsePacket();

  if (packetSize) {
    // received a packet
    Serial.print("Received ");

    if (CAN.packetExtended()) {
      Serial.print("extended ");
    }

    if (CAN.packetRtr()) {
      // Remote transmission request, packet contains no data
      Serial.print("RTR ");
    }

    Serial.print("packet with id 0x");
    Serial.print(CAN.packetId(), HEX);

    if (CAN.packetRtr()) {
      Serial.print(" and requested length ");
      Serial.println(CAN.packetDlc());
    } else {
      Serial.print(" and length ");
      Serial.println(packetSize);

      // only print packet data for non-RTR packets
      while (CAN.available()) {
        Serial.print((char)CAN.read());
      }
      Serial.println();
    }

    Serial.println();
  }

  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  //delay(500);
}
