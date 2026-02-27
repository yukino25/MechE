/*
Adafruit circuit playground express
using onboard accelerometer to detect movement and sound buzzer when movement is detected.
external RC522 RFID reader to detect when a specific RFID tag is near, disable the buzzer when the tag actives, and re-enable the buzzer when the tag actives again.


CPX toggle switch between read and write mode for RFID
*/
//*Libraries
#include <Adafruit_CircuitPlayground.h>
#include <SPI.h>
#include <MFRC522.h>
//nonvolatile memory library to store the last randomly generated key
#include <FlashStorage.h>

//*Pin numbers
#define RFID_MODE 7
#define BUZZER_PIN A0
#define RST_PIN A2
#define SS_PIN A1
#define SCK_PIN A6
#define MOSI_PIN A7
#define MISO_PIN A5


//*Variables
//accelerometer variables
float x, y, z;
float threshold = 1.5; //threshold for movement detection
//lock state variable, toggle when correct RFID tag is detected, controls whether movement detection and buzzer are active
bool lock = false; 



// Struct and flash storage slot for keeping the key across power cycles
typedef struct {
    bool valid;
    byte data[16];
} KeyStorage;
FlashStorage(key_storage, KeyStorage);

// Create MFRC522 instance
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

// Stores the last randomly written key so RUN() can verify against it
byte storedKey[16];
byte buffer[18];

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  pinMode(RFID_MODE, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  // Random number generator using floating analog pins and micros
  randomSeed(analogRead(A3) ^ (analogRead(A4) << 8) ^ micros());

  // Initialize RFID reader
  SPI.begin();
  mfrc522.PCD_Init();
  RFID_PREP();

  // Load key from flash if one has been written before
  KeyStorage ks = key_storage.read();
  if (ks.valid) {
      memcpy(storedKey, ks.data, 16);
      Serial.print(F("Key found in flash: "));
      dump_byte_array(storedKey, 16);
      Serial.println();
  }

  //read toggle switch state once at power up to set initial mode
  digitalRead(RFID_MODE);
}

void loop(){

    if (RFID_MODE == HIGH) {
        RFID_WRITE();
    }
    else {
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
    Serial.println(mfrc522.PICC_GetTypeName(piccType));

    // Check for compatibility
    if (    piccType != MFRC522::PICC_TYPE_MIFARE_MINI
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
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
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }

    // Show the whole sector as it currently is
    Serial.println(F("Current data in sector:"));
    mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
    Serial.println();

    // Read data from the block
    Serial.print(F("Reading data from block ")); Serial.print(blockAddr);
    Serial.println(F(" ..."));
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    Serial.print(F("Data in block ")); Serial.print(blockAddr); Serial.println(F(":"));
    dump_byte_array(buffer, 16); Serial.println();
    Serial.println();

    // Authenticate using key B
    Serial.println(F("Authenticating again using key B..."));
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }

    // Write data to the block
    Serial.print(F("Writing data into block ")); Serial.print(blockAddr);
    Serial.println(F(" ..."));
    dump_byte_array(dataBlock, 16); Serial.println();
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(blockAddr, dataBlock, 16);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    Serial.println();

    // Read data from the block (again, should now be what we have written)
    Serial.print(F("Reading data from block ")); Serial.print(blockAddr);
    Serial.println(F(" ..."));
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
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
    mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
    Serial.println();

    // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
}




//? function to read data from RFID, control buzzer and detect movement when toggle switch is in read mode
void RUN(){
    // only care about movement if lock is active
    if(lock){
        //* read from RFID, compare to stored key
        RFID_READ();
        // needs to either detect a card for function or detect movement
        x = CircuitPlayground.motionX();
        y = CircuitPlayground.motionY();
        z = CircuitPlayground.motionZ();
        MOVEMENT_DETECT();

        // if movement is detected
        if (MOVEMENT_DETECT()) {

        }
        //if movement isnt detected
        else if(!MOVEMENT_DETECT()){

        }
    }
    
    else if (!lock){
        //* read from RFID, compare to stored key
        RFID_READ();

    }
}


//? detect movement using accelerometer
bool MOVEMENT_DETECT(){
    if (abs(x) > threshold || abs(y) > threshold || abs(z) > threshold) {
        // Movement detected, return true
        return true;
    } else {
        // No movement, return false
      return false;
    }
}


//? read data from RFID to enable/disable "lock state" of board, control buzzer based on lock state
void RFID_READ() {

    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if ( ! mfrc522.PICC_IsNewCardPresent()){
        return;
    }

    // compare the RFID key read with the stored key, if they match toggle lock state
    if ( ! mfrc522.PICC_ReadCardSerial()){
            byte count = 0;
        for (byte i = 0; i < 16; i++) {
        // Compare buffer (= what was read) with dataBlock (= what was written)
            if (buffer[i] == dataBlock[i]){
                count++;
            }
        }
        Serial.print(F("Number of bytes that match = ")); Serial.println(count);
        if (count == 16) {
            //if the correct key is read, toggle lock state
            lock = !lock;
            Serial.println(F("key match, lock state toggled"));
        } else {
            Serial.println(F("key mismatch, lock state unchanged"));
        }
    }
}



void RFID_PREP(){
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }

    Serial.println(F("Scan a MIFARE Classic PICC to demonstrate read and write."));
    Serial.print(F("Using key (for A and B):"));
    dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
    Serial.println();

    Serial.println(F("BEWARE: Data will be written to the PICC, in sector #1"));
}

void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}
