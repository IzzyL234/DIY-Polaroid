#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include "esp_camera.h"
#include "printer_setup.h"

// Most 58mm thermal printers print around 384 pixels wide.
#define PRINTER_WIDTH_PIXELS 384

void processAndPrintImage(camera_fb_t *fb) {
  Serial.println("Processing image...");

  int originalWidth = fb->width;
  int originalHeight = fb->height;

  Serial.print("Original width: ");
  Serial.println(originalWidth);

  Serial.print("Original height: ");
  Serial.println(originalHeight);

  // This is placeholder processing logic.
  // Later, this section will:
  // 1. Resize image to printer width
  // 2. Convert grayscale to black and white
  // 3. Apply dithering
  // 4. Convert pixels into ESC/POS bitmap data
  // 5. Send bitmap bytes to printer

  Serial.println("Converting grayscale image to black and white...");

  for (int y = 0; y < originalHeight; y++) {
    for (int x = 0; x < originalWidth; x++) {
      int index = y * originalWidth + x;

      uint8_t pixel = fb->buf[index];

      // Simple black/white threshold for testing.
      bool blackPixel = pixel < 128;

      // For now, do not actually print each pixel.
      // This prevents sending broken image data during early testing.
    }
  }

  Serial.println("Image processed.");

  // Test printer output
  printerSerial.println("Photo captured.");
  printerSerial.println("Image processing placeholder complete.");
  printerSerial.println("Bitmap printing will be added later.");
  printerFeedLines(3);

  Serial.println("Sent test text to printer.");
}

#endif
