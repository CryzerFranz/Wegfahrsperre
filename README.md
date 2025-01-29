# RFID-Wegfahrsperre mit Arduino

Dieses Projekt implementiert eine einfache Wegfahrsperre mit einem RFID-Modul und einer LED. Eine LED zeigt den Status der Wegfahrsperre an: Leuchtet sie, ist die Sperre aktiv.

## Verwendete Komponenten

- **Arduino (ESP32 oder kompatibel)**
- **RFID RC522 Modul**
- **LED**
- **Widerstand (220 Ohm, optional)**
- **Jumper-Kabel**

## Optional: Installieren der Library für das Modul RFID RC522
Wenn nötig installieren Sie die Library über den folgenden Link: <br>
https://www.arduinolibraries.info/libraries/mfrc522 <br>
Fügen Sie den Ordner in Ihren Projekt unter dem Verzeichnis Lib.

## Schaltplan

| RFID RC522 | ESP32   |
| ---------- | ------- |
| VCC        | 3.3V    |
| GND        | GND     |
| RST        | GPIO 22 |
| SDA(SS)    | GPIO 5  |
| MOSI       | GPIO 23 |
| MISO       | GPIO 19 |
| SCK        | GPIO 18 |

| LED         | ESP32   |
| ----------- | ------- |
| Anode (+)   | GPIO 25 |
| Kathode (-) | GND     |

## Funktionsweise

1. Das Programm initialisiert den RFID-Reader und die LED.
2. Wenn eine RFID-Karte in Reichweite kommt, wird ihre UID ausgelesen.
3. Falls die UID mit der gespeicherten erlaubten UID übereinstimmt, wird die LED geschaltet:
   - Falls die Wegfahrsperre aktiviert ist, ist die LED aktiviert (LED an).
   - Falls die Wegfahrsperre deaktiviert ist, ist die LED deaktiviert (LED aus).
4. Falls die UID nicht erkannt wird, bleibt die Wegfahrsperre aktiv (LED an).

## Code-Erklärung

### Bibliotheken und Definitionen

```cpp
#include <SPI.h>
#include <Arduino.h>
#include <MFRC522.h>

#define RST_PIN 22    // Reset-Pin des RFID-Moduls
#define SS_PIN 5      // Slave-Select-Pin des RFID-Moduls
#define LED_PIN 25    // Pin für die LED
```

Hier werden die benötigten Bibliotheken eingebunden und die verwendeten Pins festgelegt.

### RFID-Initialisierung

```cpp
MFRC522 rfid(SS_PIN, RST_PIN);
byte allowedUID[] = {0xF5, 0x5E, 0x9, 0x1};
```

Ein RFID-Objekt wird erstellt und eine erlaubte UID definiert.

### UID-Überprüfung

```cpp
bool checkUID(byte *uid, byte length) {
  if (length != sizeof(allowedUID)) {
    return false;
  }
  for (byte i = 0; i < length; i++) {
    if (uid[i] != allowedUID[i]) {
      return false;
    }
  }
  return true;
}
```

Diese Funktion vergleicht die gescannte UID mit der gespeicherten erlaubten UID.

### LED-Steuerung

```cpp
typedef enum { ACTIVATED, DEACTIVATED } State_E;
State_E state = ACTIVATED;
```

Ein einfacher Zustandsautomat speichert den Status der Wegfahrsperre.

### Setup-Funktion

```cpp
void setup() {
  Serial.begin(115200);
  SPI.begin(18, 19, 23, 5);
  rfid.PCD_Init();
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  Serial.println("Wegfahrsperre gestartet. Bitte scannen Sie eine Karte.");
}
```

Hier wird die serielle Kommunikation gestartet, das RFID-Modul initialisiert und die LED in den gesperrten Zustand gesetzt.

### Loop-Funktion

```cpp
void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }
```

Wenn eine neue Karte erkannt wird, wird ihre UID ausgelesen.

```cpp
  if (checkUID(rfid.uid.uidByte, rfid.uid.size)) {
    if (state == ACTIVATED) {
      Serial.println("Karte erkannt. Wegfahrsperre deaktiviert.");
      state = DEACTIVATED;
      digitalWrite(LED_PIN, LOW);
    } else {
      digitalWrite(LED_PIN, HIGH);
      state = ACTIVATED;
      Serial.println("Karte erkannt. Wegfahrsperre aktiviert.");
    }
  } else {
    Serial.println("Karte nicht erkannt. Zugriff verweigert.");
  }
  rfid.PICC_HaltA();
}
```

Wenn die UID übereinstimmt, wird die LED entsprechend geschaltet. Falls nicht, bleibt die Wegfahrsperre aktiv.

## Fazit

Dieses Projekt kann als einfache Wegfahrsperre oder Zugangskontrolle genutzt werden. Es ist leicht anpassbar, indem weitere erlaubte UIDs im Code gespeichert werden.

---

