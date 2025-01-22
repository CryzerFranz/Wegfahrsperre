#include <SPI.h>
#include <Arduino.h>
#include <MFRC522.h>

#define RST_PIN 22    // Reset-Pin des RFID-Moduls
#define SS_PIN 5      // Slave-Select-Pin des RFID-Moduls
#define RELAY_PIN 25  // Pin für das Relais

MFRC522 rfid(SS_PIN, RST_PIN);

// Erlaubte UID der RFID-Karte (hier ein Beispiel: 4 Byte)
byte allowedUID[] = {0xF5, 0x5E, 0x9, 0x1};

// Funktion zum Überprüfen der UID
bool checkUID(byte *uid, byte length) {
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

 typedef enum{
    ACTIVATED, DEACTIVATED } State_E;
  State_E state = ACTIVATED;

void setup() {
  
  Serial.begin(115200);         // Serielle Kommunikation starten
  SPI.begin(18, 19, 23, 5);     // Initialisierung der SPI-Schnittstelle
  rfid.PCD_Init();              // Initialisierung des RFID-Readers
  pinMode(RELAY_PIN, OUTPUT);   // Relais-Pin als Ausgang setzen
  digitalWrite(RELAY_PIN, HIGH); // Relais deaktivieren (Wegfahrsperre aktiv)
  Serial.println("Wegfahrsperre gestartet. Bitte scannen Sie eine Karte.");
}

void loop() {
  // Überprüfen, ob eine Karte in Reichweite ist
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  // UID der Karte auslesen
  Serial.print("Gescannte UID: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  // UID überprüfen
  if (checkUID(rfid.uid.uidByte, rfid.uid.size)) {
    if(state == ACTIVATED)
    {
      Serial.println("Karte erkannt. Wegfahrsperre deaktiviert.");
      state = DEACTIVATED;
      digitalWrite(RELAY_PIN, LOW);  // Relais deaktivieren
    }else
    {
      digitalWrite(RELAY_PIN, HIGH); // Relais aktivieren
      state = ACTIVATED;
      Serial.println("Karte erkannt. Wegfahrsperre aktviert.");
    }
  } else {
    state = ACTIVATED;
    Serial.println("Karte nicht erkannt. Zugriff verweigert.");
  }

  // Karte abhängen
  rfid.PICC_HaltA();
}


