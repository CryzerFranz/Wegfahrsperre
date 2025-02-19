#ifndef RFID_CP_H
#define RFID_CP_H

#include <Arduino.h>
#include <MFRC522.h>
#include <SPI.h>

#define RST_PIN 22    // Reset-Pin des RFID-Moduls
#define SS_PIN 5      // Slave-Select-Pin des RFID-Moduls
#define RELAY_PIN 25  // Pin für das Relais

typedef enum { ACTIVATED, DEACTIVATED } State_E;

class RFID_CP {
private:
    byte* allowedUID;
    byte size;
    State_E state;
    bool checkUID(byte* uid, byte length);
    MFRC522 rfid;

public:
    RFID_CP(byte* id, byte length);
    bool isTransponderNear();
    bool readTransponder();
    void clear();
};

#endif // RFID_CP_H
