#include <SPI.h>
#include <Arduino.h>
#include <MFRC522.h>
#include "rfid.h"

#define RST_PIN 22    // Reset-Pin des RFID-Moduls
#define SS_PIN 5      // Slave-Select-Pin des RFID-Moduls
#define RELAY_PIN 25  // Pin für das Relais

// Erlaubte UID der RFID-Karte (hier ein Beispiel: 4 Byte)
byte allowedUID[] = {0xF5, 0x5E, 0x9, 0x1};
RFID_CP cpRFID(allowedUID, 4);


void setup() {
  Serial.begin(115200);         // Serielle Kommunikation starten          
  pinMode(RELAY_PIN, OUTPUT);   // Relais-Pin als Ausgang setzen
  digitalWrite(RELAY_PIN, HIGH); // Relais deaktivieren (Wegfahrsperre aktiv)
  Serial.println("Wegfahrsperre gestartet. Bitte scannen Sie eine Karte.");
}

void loop() {
 if(!cpRFID.isTransponderNear())
 {
    return;
 }
 
 if(cpRFID.readTransponder())
 {
  Serial.println("Korrekt");
 }
 else{
  Serial.println("Falsch");
 }

 cpRFID.clear();
}



