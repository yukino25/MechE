/*
  SPI Loopback Test for Circuit Playground Express
  ------------------------------------------------
  Hardware setup: short A2 (MISO) to A3 (MOSI) with a jumper wire.
  Leave A1 (SCK), A7 (SS), and A6 (RST) unconnected.

  Expected output if SPI is working:
    TX: 0x55  RX: 0x55  PASS
    TX: 0xAA  RX: 0xAA  PASS
    TX: 0x12  RX: 0x12  PASS
    TX: 0xFF  RX: 0xFF  PASS
    TX: 0x00  RX: 0x00  PASS
    All 5 passed.

  If every RX is 0xFF the MISO line is floating high (bad connection or PINCFG INEN not set).
  If every RX is 0x00 the MISO line is held low (short to GND).
  If RX is always the same wrong byte, the MUX is still on a wrong peripheral.
*/

#include <SPI.h>
#include "wiring_private.h"

#define SCK_PIN  A1   // SERCOM0/PAD1  PA05
#define MISO_PIN A2   // SERCOM0/PAD2  PA06
#define MOSI_PIN A3   // SERCOM0/PAD3  PA07
#define SS_PIN   A7   // GPIO chip-select (held HIGH throughout test)

SPIClass rfidSPI(&sercom0, MISO_PIN, SCK_PIN, MOSI_PIN, SPI_PAD_3_SCK_1, SERCOM_RX_PAD_2);

static const byte TEST_BYTES[] = { 0x55, 0xAA, 0x12, 0xFF, 0x00 };
static const byte N = sizeof(TEST_BYTES);

void setup() {
  Serial.begin(9600);
  unsigned long t = millis();
  while (!Serial && millis() - t < 3000);

  pinMode(SS_PIN, OUTPUT);
  digitalWrite(SS_PIN, HIGH);

  rfidSPI.begin();

  // Mux A1/A2/A3 to SERCOM0 (MUX D = 0x3) — same register writes as BoardLock
  PORT->Group[PORTA].PINCFG[5].reg |= PORT_PINCFG_PMUXEN;
  PORT->Group[PORTA].PMUX[2].reg    = (PORT->Group[PORTA].PMUX[2].reg & 0x0F) | 0x30;
  PORT->Group[PORTA].PINCFG[6].reg  = PORT_PINCFG_PMUXEN | PORT_PINCFG_INEN;
  PORT->Group[PORTA].PINCFG[7].reg |= PORT_PINCFG_PMUXEN;
  PORT->Group[PORTA].PMUX[3].reg    = 0x33;

  rfidSPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));

  byte passed = 0;
  for (byte i = 0; i < N; i++) {
    byte tx = TEST_BYTES[i];
    byte rx = rfidSPI.transfer(tx);
    Serial.print(F("TX: 0x")); if (tx < 0x10) Serial.print('0'); Serial.print(tx, HEX);
    Serial.print(F("  RX: 0x")); if (rx < 0x10) Serial.print('0'); Serial.print(rx, HEX);
    if (rx == tx) {
      Serial.println(F("  PASS"));
      passed++;
    } else {
      Serial.println(F("  FAIL"));
    }
  }

  rfidSPI.endTransaction();

  Serial.println();
  if (passed == N) {
    Serial.println(F("All passed. SPI bus is functional — problem is RC522 wiring/device."));
  } else {
    Serial.print(passed); Serial.print('/'); Serial.print(N);
    Serial.println(F(" passed. SPI bus itself is broken — check SCK/MOSI/MISO wiring to CPX pads."));
  }
}

void loop() {}
