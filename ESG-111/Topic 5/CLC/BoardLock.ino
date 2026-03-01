/*
Adafruit circuit playground express
using onboard accelerometer to detect movement and sound buzzer when movement is detected.
external RC522 RFID reader to detect when a specific RFID tag is near, disable the buzzer when the tag actives, and re-enable the buzzer when the tag actives again.


CPX toggle switch between read and write mode for RFID
*/
//*Libraries
#include <Adafruit_CircuitPlayground.h>
#include <SPI.h>
#include "wiring_private.h"  // pinPeripheral() + sercom0 for custom SPI
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>
//nonvolatile memory library to store the last randomly generated key
#include <FlashStorage.h>


//*Pin numbers
#define RFID_MODE 7
#define BUZZER_PIN A0
// Hardware SPI on SERCOM0 — the only valid external SPI interface on CPX edge pads
// Default SPI uses SERCOM3 (internal flash), so a custom SPIClass is required
#define SS_PIN   A7  // chip select: PORTB GPIO (PB08)
#define SCK_PIN  A1  // SERCOM0/PAD1 (PA05)
#define MOSI_PIN A3  // SERCOM0/PAD3 (PA07)
#define MISO_PIN A2  // SERCOM0/PAD2 (PA06)
#define RST_PIN  A6  // RC522 reset: drive HIGH to keep chip active (PB09, free GPIO)

//*Timing constants
#define HOLD_DURATION  2000UL   // ms card must be held continuously to toggle mode (vs tap to lock)
#define LOCK_GRACE_MS 10000UL   // ms after locking before alarm can activate
#define ALARM_STOP_MS 10000UL   // ms of no movement before alarm silences itself
#define AUTO_LOCK_MS  10000UL   // ms of no movement before auto-lock engages

//*Variables
//accelerometer variables
float x, y, z;
const float threshold = 1.5; //threshold for movement detection
//lock state variable, toggle when correct RFID tag is detected, controls whether movement detection and buzzer are active
bool lock = false;
bool readmode;
// Operating mode: false = manual (tap to lock/unlock), true = auto (lock when no movement)
bool autoLockMode = false;

// Alarm / auto-lock timing
unsigned long lastMovementTime = 0;  // last millis() movement was detected (unlocked, auto mode)
unsigned long lockGraceEnd    = 0;   // alarm won't activate until after this timestamp
bool          alarmSounding   = false;
unsigned long noMovementSince = 0;   // when continuous no-movement started (while alarm is on)

// Card tap-vs-hold detection state
bool          cardPending      = false;
unsigned long cardDetectedTime = 0;

float magnitude;


// Struct and flash storage slot for keeping the key across power cycles
typedef struct {
    bool valid;
    byte data[16];
} KeyStorage;
FlashStorage(key_storage, KeyStorage);

// Custom SPI on SERCOM0 using CPX edge pads A1(SCK)/A3(MOSI)/A2(MISO)
// Constructor: SPIClass(sercom, miso_pin, sck_pin, mosi_pin, tx_pad, rx_pad)
// SPI_PAD_3_SCK_1 → MOSI=PAD3(A3), SCK=PAD1(A1) | SERCOM_RX_PAD_2 → MISO=PAD2(A2)
SPIClass rfidSPI(&sercom0, MISO_PIN, SCK_PIN, MOSI_PIN, SPI_PAD_3_SCK_1, SERCOM_RX_PAD_2);

// Create MFRC522 instance using v2 SPI driver pattern (RST handled via software reset)
// Explicit SPISettings needed: SAMD SPI_CLOCK_DIV4=12 is a raw divider, not Hz —
// passing it as a frequency to SPISettings causes BAUD register overflow → garbled clock.
MFRC522DriverPinSimple ss_pin(SS_PIN);
MFRC522DriverSPI driver{ss_pin, rfidSPI, SPISettings(2000000, MSBFIRST, SPI_MODE0)};
MFRC522 mfrc522{driver};
MFRC522::MIFARE_Key key;

// Stores the last randomly written key so RUN() can verify against it
byte storedKey[16];
byte dataBlock[16];
byte buffer[18];



void setup() {
  Serial.begin(9600);
  // Wait up to 3 s for serial monitor; continue anyway so RFID works standalone
  unsigned long _t = millis();
  while (!Serial && millis() - _t < 3000);
  CircuitPlayground.begin();
  pinMode(RFID_MODE, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Random number generator (A6/A5 used here before RST_PIN is configured as OUTPUT)
  randomSeed(analogRead(A6) ^ (analogRead(A5) << 8) ^ micros());

  // Drive RST and SS to known-good states before the SPI bus is touched
  // SS must be HIGH (deselected) before rfidSPI.begin() or the RC522 sees bus glitches
  pinMode(SS_PIN,  OUTPUT); digitalWrite(SS_PIN,  HIGH);
  pinMode(RST_PIN, OUTPUT); digitalWrite(RST_PIN, HIGH);
  delay(50);  // RC522 needs ~50 ms after RST goes HIGH before SPI is valid

  // Initialize RFID reader on SERCOM0 (A1=SCK, A2=MISO, A3=MOSI, A7=SS)
  rfidSPI.begin();
  // Directly mux A1(PA05)/A2(PA06)/A3(PA07) to SERCOM0 peripheral C (MUX value 0x2).
  // rfidSPI.begin() leaves them on PIO_ANALOG; pinPeripheral() relies on the Adafruit
  // variant library which may behave differently — write the PORT registers directly.
  //   PMUX[n] holds two pins: even pin in bits[3:0], odd pin in bits[7:4]
  //   SERCOM0 for PA04-PA07 is on MUX column D (peripheral D = 0x3), NOT column C.
  //   Column C on these pins is AC/AIN (Analog Comparator) — previous 0x2 was wrong.
  //   PA05 = odd  → PMUX[2] bits[7:4]; set to 0x3, preserve PA04 bits[3:0]
  //   PA06 = even → PMUX[3] bits[3:0]; set to 0x3
  //   PA07 = odd  → PMUX[3] bits[7:4]; set to 0x3
  PORT->Group[PORTA].PINCFG[5].reg |= PORT_PINCFG_PMUXEN;
  PORT->Group[PORTA].PMUX[2].reg    = (PORT->Group[PORTA].PMUX[2].reg & 0x0F) | 0x30;
  PORT->Group[PORTA].PINCFG[6].reg  = PORT_PINCFG_PMUXEN | PORT_PINCFG_INEN; // INEN for MISO
  PORT->Group[PORTA].PINCFG[7].reg |= PORT_PINCFG_PMUXEN;
  PORT->Group[PORTA].PMUX[3].reg    = 0x33;  // PA06 even=0x3 (SERCOM0/PAD2), PA07 odd=0x3 (SERCOM0/PAD3)
  // Force SERCOM0 hardware config to run at 2 MHz before PCD_Init touches the bus.
  // Without this, Adafruit SAMD may skip initSPI() if the stored clock == default clock.
  rfidSPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
  rfidSPI.endTransaction();
  mfrc522.PCD_Init();
  RFID_PREP();
  // Prints firmware version: 0x91/0x92 = good. 0x00 or 0xFF = SPI fault (check wiring).
  MFRC522Debug::PCD_DumpVersionToSerial(mfrc522, Serial);
  // Load key from flash if one has been written before
  KeyStorage ks = key_storage.read();
  if (ks.valid) {
      memcpy(storedKey, ks.data, 16);
      Serial.print(F("Key found in flash: "));
      dump_byte_array(storedKey, 16);
      Serial.println();
  }

  //read toggle switch state once at power up to set initial mode
  readmode = digitalRead(RFID_MODE);
}

void loop(){

    if (readmode) {
        RFID_WRITE();
    }
    else if (!readmode) {
        RUN();
    }
}



//? function to write data to RFID tag when toggle switch is in write mode
//? function taken from example code provided by library, modified to fit specific requirements
//? used in production to create unique key for board
void RFID_WRITE() {
    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if ( ! mfrc522.PICC_IsNewCardPresent()){
        return;
    }

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()){
        return;
    }
    // Show some details of the PICC (tag/card)
    Serial.print(F("Card UID:"));
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();
    Serial.print(F("PICC type: "));
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(MFRC522Debug::PICC_GetTypeName(piccType));

    // Check for compatibility
    if (    piccType != MFRC522Constants::PICC_TYPE_MIFARE_MINI
        &&  piccType != MFRC522Constants::PICC_TYPE_MIFARE_1K
        &&  piccType != MFRC522Constants::PICC_TYPE_MIFARE_4K) {
        //only works with MIFARE Classic cards
        return;
    }

    // In this sample we use the second sector,
    // that is: sector #1, covering block #4 up to and including block #7
    byte sector         = 1;
    byte blockAddr      = 4;

    // Generate a random 16-byte key and store it for later verification in RUN()

    for (byte i = 0; i < 16; i++) {
        dataBlock[i] = random(0, 256);
    }
    memcpy(storedKey, dataBlock, 16);

    //* Save to flash so the key survives power cycles
    KeyStorage ks;
    ks.valid = true;
    memcpy(ks.data, storedKey, 16);
    key_storage.write(ks);

    Serial.print(F("Generated key: "));
    dump_byte_array(dataBlock, 16);
    Serial.println();
    byte trailerBlock   = 7;
    MFRC522::StatusCode status;
    byte buffer[18];
    byte size = sizeof(buffer);

    // Authenticate using key A
    Serial.println(F("Authenticating using key A..."));
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522Constants::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522Constants::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(MFRC522Debug::GetStatusCodeName(status));
        return;
    }

    // Show the whole sector as it currently is
    Serial.println(F("Current data in sector:"));
    MFRC522Debug::PICC_DumpMifareClassicSectorToSerial(mfrc522, Serial, &mfrc522.uid, &key, sector);
    Serial.println();

    // Read data from the block
    Serial.print(F("Reading data from block ")); Serial.print(blockAddr);
    Serial.println(F(" ..."));
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    if (status != MFRC522Constants::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(MFRC522Debug::GetStatusCodeName(status));
    }
    Serial.print(F("Data in block ")); Serial.print(blockAddr); Serial.println(F(":"));
    dump_byte_array(buffer, 16); Serial.println();
    Serial.println();

    // Authenticate using key B
    Serial.println(F("Authenticating again using key B..."));
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522Constants::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522Constants::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(MFRC522Debug::GetStatusCodeName(status));
        return;
    }

    // Write data to the block
    Serial.print(F("Writing data into block ")); Serial.print(blockAddr);
    Serial.println(F(" ..."));
    dump_byte_array(dataBlock, 16); Serial.println();
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(blockAddr, dataBlock, 16);
    if (status != MFRC522Constants::STATUS_OK) {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(MFRC522Debug::GetStatusCodeName(status));
    }
    Serial.println();

    // Read data from the block (again, should now be what we have written)
    Serial.print(F("Reading data from block ")); Serial.print(blockAddr);
    Serial.println(F(" ..."));
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    if (status != MFRC522Constants::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(MFRC522Debug::GetStatusCodeName(status));
    }
    Serial.print(F("Data in block ")); Serial.print(blockAddr); Serial.println(F(":"));
    dump_byte_array(buffer, 16); Serial.println();

    //* Check that data in block is what was have written by counting the number of bytes that are equal
    Serial.println(F("Checking result..."));
    byte count = 0;
    for (byte i = 0; i < 16; i++) {
        // Compare buffer (= what was read) with dataBlock (= what was written)
        if (buffer[i] == dataBlock[i]){
            count++;
        }
    }
    Serial.print(F("Number of bytes that match = ")); Serial.println(count);
    if (count == 16) {
        Serial.println(F("Success"));
    } else {
        Serial.println(F("Failure, no match"));
    }
    Serial.println();

    // Dump the sector data
    Serial.println(F("Current data in sector:"));
    MFRC522Debug::PICC_DumpMifareClassicSectorToSerial(mfrc522, Serial, &mfrc522.uid, &key, sector);
    Serial.println();

    // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
}




//? main run loop: card handling, alarm, and auto-lock
void RUN(){
    // Resolve any pending card tap/hold interaction
    CHECK_CARD_HOLD();

    // Only scan for a new card when no interaction is already in progress
    if (!cardPending) {
        RFID_READ();
    }

    // Read accelerometer once per loop iteration
    x = CircuitPlayground.motionX();
    y = CircuitPlayground.motionY();
    z = CircuitPlayground.motionZ();
    Serial.print(x);
    Serial.print(", ");
    Serial.print(y);
    Serial.print(", ");
    Serial.println(z);
    if (lock) {
        ALARM_UPDATE();
    } else {
        AUTO_LOCK_CHECK();
    }
}


//? detect movement using accelerometer
bool MOVEMENT_DETECT(){
    magnitude = sqrt(x*x + y*y + z*z);
    if (abs(magnitude - 9.8) > threshold) {
        // Movement detected, return true
        return true;
    } else {
        // No movement, return false
      return false;
    }
}


//? determine whether a valid card scan was a short tap (toggle lock) or a 2-second hold (toggle mode)
//?   - tap  < HOLD_DURATION : toggle lock state
//?   - hold >= HOLD_DURATION: toggle operating mode (auto / manual), play 2 or 3 beeps
void CHECK_CARD_HOLD() {
    if (!cardPending) return;

    unsigned long elapsed = millis() - cardDetectedTime;

    // PICC_WakeupA sends WUPA, wakes halted cards
    byte bufferATQA[2];
    byte bufferSize = sizeof(bufferATQA);
    MFRC522::StatusCode result = mfrc522.PICC_WakeupA(bufferATQA, &bufferSize);

    if (result == MFRC522Constants::STATUS_OK) {
        // Card is still present — re-select then re-halt to reset its RF state
        if (mfrc522.PICC_ReadCardSerial()) {
            mfrc522.PICC_HaltA();
            mfrc522.PCD_StopCrypto1();
        }

        if (elapsed >= HOLD_DURATION) {
            // 2-second hold confirmed: toggle operating mode
            cardPending   = false;
            autoLockMode  = !autoLockMode;
            //number of beeps based on mode: 2 for auto, 3 for manual
            int beepCount = autoLockMode ? 2 : 3;
            for (int i = 0; i < beepCount; i++) {
                tone(BUZZER_PIN, 1000, 150);
                delay(250);
            }
            Serial.println(autoLockMode ? F("Mode: AUTO (no-motion lock)") : F("Mode: MANUAL (tap to lock)"));
        }
        // else: still under 2 seconds — keep waiting

    } else {
        // Card is not present
        cardPending = false;

        if (elapsed < HOLD_DURATION) {
            // Short tap: toggle lock state
            lock = !lock;
            if (lock) {
                lockGraceEnd  = millis() + LOCK_GRACE_MS;
                noMovementSince = 0;
                alarmSounding   = false;
                Serial.println(F("LOCKED"));
            } else {
                if (alarmSounding) {
                    noTone(BUZZER_PIN);
                    alarmSounding = false;
                }
                lastMovementTime = 0;
                Serial.println(F("UNLOCKED"));
            }
        } else {
            // Card was held long enough but left before the next CHECK_CARD_HOLD poll — still toggle mode
            autoLockMode  = !autoLockMode;
            int beepCount = autoLockMode ? 2 : 3;
            for (int i = 0; i < beepCount; i++) {
                tone(BUZZER_PIN, 1000, 150);
                delay(250);
            }
            Serial.println(autoLockMode ? F("Mode: AUTO (no-motion lock)") : F("Mode: MANUAL (tap to lock)"));
        }
    }
}


//? manage alarm while board is locked:
//?   - alarm starts when movement is detected (after grace period)
//?   - alarm silences after ALARM_STOP_MS of no movement
//?   - alarm restarts if movement is detected again
void ALARM_UPDATE() {
    // Grace period after locking: gives user time to set the board down without triggering alarm
    if (millis() < lockGraceEnd) return;

    if (MOVEMENT_DETECT()) {
        noMovementSince = 0;
        if (!alarmSounding) {
            alarmSounding = true;
            tone(BUZZER_PIN, 2000);   // continuous tone until noTone() is called
        }
    } else {
        if (alarmSounding) {
            if (noMovementSince == 0) {
                noMovementSince = millis();
            }
            if (millis() - noMovementSince >= ALARM_STOP_MS) {
                // 10 seconds of no movement: silence alarm
                alarmSounding   = false;
                noMovementSince = 0;
                noTone(BUZZER_PIN);
            }
        }
    }
}


//? auto-lock after AUTO_LOCK_MS of no movement (auto mode only, board must be unlocked)
void AUTO_LOCK_CHECK() {
    if (!autoLockMode) return;

    if (MOVEMENT_DETECT()) {
        lastMovementTime = millis();
    } else if (lastMovementTime > 0 && millis() - lastMovementTime >= AUTO_LOCK_MS) {
        lock            = true;
        lockGraceEnd    = millis() + LOCK_GRACE_MS;
        lastMovementTime = 0;
        noMovementSince  = 0;
        alarmSounding    = false;
        Serial.println(F("AUTO LOCKED: no motion detected"));
    }
}


//? read data from RFID to enable/disable "lock state" of board, control buzzer based on lock state
void RFID_READ() {

    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if ( ! mfrc522.PICC_IsNewCardPresent()){
        return;
    }

    if ( ! mfrc522.PICC_ReadCardSerial()){
        return;
    }

    // Authenticate and read block 4 into buffer, then compare to stored key
    byte trailerBlock = 7;
    byte blockAddr    = 4;
    byte size         = sizeof(buffer);
    MFRC522::StatusCode status;

    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522Constants::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522Constants::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(MFRC522Debug::GetStatusCodeName(status));
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();
        return;
    }

    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    if (status != MFRC522Constants::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(MFRC522Debug::GetStatusCodeName(status));
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();
        return;
    }

    // Compare buffer (what was read) with storedKey (what was written)
    byte count = 0;
    for (byte i = 0; i < 16; i++) {
        if (buffer[i] == storedKey[i]) {
            count++;
        }
    }
    Serial.print(F("Number of bytes that match = ")); Serial.println(count);
    if (count == 16) {
        // Valid card: halt it and start the tap/hold timer
        cardPending      = true;
        cardDetectedTime = millis();
        Serial.println(F("Valid card — timing tap vs hold..."));
    } else {
        Serial.println(F("key mismatch, lock state unchanged"));
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
}



void RFID_PREP(){
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }

    Serial.println(F("Scan a MIFARE Classic PICC to demonstrate read and write."));
    Serial.print(F("Using key (for A and B):"));
    dump_byte_array(key.keyByte, MFRC522Constants::MF_KEY_SIZE);
    Serial.println();

    Serial.println(F("BEWARE: Data will be written to the PICC, in sector #1"));

}

void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}
