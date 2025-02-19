#include <Arduino.h>
#include "rfid.h"

RFID_CP::RFID_CP(byte *id, byte length)
{
    this->allowedUID = id;
    this->size = length;
    this->state = ACTIVATED;
    SPI.begin(18, 19, 23, 5);     // Initialisierung der SPI-Schnittstelle
    rfid.PCD_Init();
    MFRC522 rfid(SS_PIN, RST_PIN);
}

bool RFID_CP::isTransponderNear()
{
    if (!RFID_CP::rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) 
    {
        return false;
    }
    return true;
}

bool RFID_CP::readTransponder()
{
    Serial.print("Gescannte UID: ");
    for (byte i = 0; i < rfid.uid.size; i++) 
    {
        Serial.print(rfid.uid.uidByte[i], HEX);
        Serial.print(" ");
    }
    Serial.println();

    return checkUID(rfid.uid.uidByte, rfid.uid.size);
}

void RFID_CP::clear()
{
  rfid.PICC_HaltA();
}

bool RFID_CP::checkUID(byte *uid, byte length)
{
    if (length != sizeof(allowedUID)) {
    return false; // Falsche Länge
  }

  for (byte i = 0; i < length; i++) {
    if (uid[i] != allowedUID[i]) {
      return false; // UID stimmt nicht überein
    }
  }
  
  return true; // UID ist korrekt
}

