#include "camera_setup.h"
#include "printer_setup.h"
#include "image_processing.h"

#define SHUTTER_BUTTON_PIN 13

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(SHUTTER_BUTTON_PIN, INPUT_PULLUP);

  setupCamera();
  setupPrinter();

  Serial.println("DIY Polaroid test code ready.");
}

void loop() {
  if (digitalRead(SHUTTER_BUTTON_PIN) == LOW) {
    Serial.println("Button pressed. Capturing photo...");

    camera_fb_t *fb = esp_camera_fb_get();

    if (!fb) {
      Serial.println("Camera capture failed.");
      return;
    }

    Serial.println("Photo captured.");

    processAndPrintImage(fb);

    esp_camera_fb_return(fb);

    Serial.println("Done printing. Waiting for next button press.");

    delay(1000);
  }
}
