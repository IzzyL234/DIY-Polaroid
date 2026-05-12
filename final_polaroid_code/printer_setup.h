#ifndef PRINTER_SETUP_H
#define PRINTER_SETUP_H

#include <HardwareSerial.h>

// Change these pins depending on your wiring.
#define PRINTER_RX_PIN 14
#define PRINTER_TX_PIN 15

HardwareSerial printerSerial(1);

void setupPrinter() {
  printerSerial.begin(9600, SERIAL_8N1, PRINTER_RX_PIN, PRINTER_TX_PIN);

  delay(500);

  // ESC/POS initialize printer command
  printerSerial.write(27);
  printerSerial.write(64);

  Serial.println("Thermal printer serial initialized.");
}

void printerFeedLines(int lines) {
  for (int i = 0; i < lines; i++) {
    printerSerial.write('\n');
  }
}

#endif
