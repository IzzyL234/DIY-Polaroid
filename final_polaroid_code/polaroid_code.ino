#include "esp_camera.h"
#include "esp_sleep.h"

#define CAMERA_MODEL_WROVER_KIT
#include "camera_pins.h"

HardwareSerial printerSerial(1);

// IMPORTANT: use GPIO13, not GPIO7.
// GPIO13 supports deep sleep wake.
const int buttonPin = 13;

const unsigned long HOLD_TIME = 2000; // 2 seconds

bool buttonWasDown = false;
unsigned long buttonDownStart = 0;
bool longPressHandled = false;

void enterOffMode() {
  Serial.println("Turning off / entering deep sleep...");
  delay(300);

  // Wake up when buttonPin goes LOW
  esp_sleep_enable_ext0_wakeup((gpio_num_t)buttonPin, 0);

  esp_deep_sleep_start();
}

bool buttonHeldForTwoSeconds() {
  unsigned long startTime = millis();

  while (digitalRead(buttonPin) == LOW) {
    if (millis() - startTime >= HOLD_TIME) {
      return true;
    }
    delay(10);
  }

  return false;
}

void waitForButtonRelease() {
  while (digitalRead(buttonPin) == LOW) {
    delay(10);
  }
  delay(200); // debounce
}

void setup() {
  Serial.begin(115200);

  pinMode(buttonPin, INPUT_PULLUP);

  /*
    Battery always connected behavior:

    - When battery is first connected, the ESP32 starts.
    - If the button is NOT being held for 2 seconds, it goes to sleep.
    - To turn on, hold the shutter button for 2 seconds.
  */

  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
    Serial.println("Woke up from button press.");

    if (!buttonHeldForTwoSeconds()) {
      Serial.println("Button was not held long enough. Going back to sleep.");
      enterOffMode();
    }

    Serial.println("Held for 2 seconds. Turning on.");
    waitForButtonRelease();
  } 
  else {
    Serial.println("Cold boot / battery connected.");

    if (!buttonHeldForTwoSeconds()) {
      Serial.println("Button was not held for startup. Going to sleep.");
      enterOffMode();
    }

    Serial.println("Startup hold detected. Turning on.");
    waitForButtonRelease();
  }

  camera_config_t config;

  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;

  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;

  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;

  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;

  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;

  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_GRAYSCALE;
  config.frame_size = FRAMESIZE_CIF;
  config.fb_count = 1;
  config.grab_mode = CAMERA_GRAB_LATEST;

  esp_err_t err = esp_camera_init(&config);

  if (err != ESP_OK) {
    Serial.println("Camera init failed");
    return;
  }

  printerSerial.begin(9600, SERIAL_8N1, -1, 5);
  delay(3000);

  Serial.println("Device is on. Short press = capture. Hold 2 sec = off.");
}

void loop() {
  bool buttonDown = digitalRead(buttonPin) == LOW;

  if (buttonDown && !buttonWasDown) {
    // button was just pressed
    buttonWasDown = true;
    buttonDownStart = millis();
    longPressHandled = false;
  }

  if (buttonDown && buttonWasDown) {
    // button is being held
    if (!longPressHandled && millis() - buttonDownStart >= HOLD_TIME) {
      longPressHandled = true;
      enterOffMode();
    }
  }

  if (!buttonDown && buttonWasDown) {
    // button was released
    unsigned long pressLength = millis() - buttonDownStart;
    buttonWasDown = false;

    if (!longPressHandled && pressLength < HOLD_TIME) {
      Serial.println("Short press: capture and print");
      takePhotoAndPrint();
    }

    delay(200); // debounce
  }

  delay(20);
}

void takePhotoAndPrint() {
  camera_fb_t* fb = takePhoto();
  if (fb == NULL) {
    Serial.println("Photo failed");
    return;
  }

  uint8_t* resized = resizeImage(fb);
  uint8_t* dithered = applyDithering(resized, 384, 288);
  uint8_t* bitmap = convertImage(dithered, 384, 288);

  printImage(bitmap, 384, 288);

  esp_camera_fb_return(fb);

  // dithered points to the same memory as resized,
  // so only delete resized once.
  delete[] resized;
  delete[] bitmap;

  delay(1000);
}

bool ifButtonPressed() {
  return digitalRead(buttonPin) == LOW;
}

camera_fb_t* takePhoto() {
  camera_fb_t* fb = esp_camera_fb_get();
  return fb;
}

uint8_t* resizeImage(camera_fb_t *input) {
  int originalWidth = input->width;
  int originalHeight = input->height;
  int newWidth = 384;
  int newHeight = 288;

  uint8_t *resizedImage = new uint8_t[newWidth * newHeight];

  for (int y = 0; y < newHeight; y++) {
    for (int x = 0; x < newWidth; x++) {
      int sourceX = x * originalWidth / newWidth;
      int sourceY = y * originalHeight / newHeight;

      resizedImage[y * newWidth + x] =
        input->buf[sourceY * originalWidth + sourceX];
    }
  }

  return resizedImage;
}

uint8_t* applyDithering(uint8_t *img, int width, int height) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int i = y * width + x;

      int oldPixel = img[i];

      int newPixel;

      if (oldPixel < 128)
        newPixel = 0;
      else
        newPixel = 255;

      img[i] = newPixel;

      int error = oldPixel - newPixel;
      int spread = error / 8;

      auto addError = [&](int xx, int yy) {
        if (xx < 0 || xx >= width || yy < 0 || yy >= height)
          return;

        int index = yy * width + xx;

        int value = img[index] + spread;

        if (value < 0)
          value = 0;

        if (value > 255)
          value = 255;

        img[index] = value;
      };

      addError(x + 1, y);
      addError(x + 2, y);

      addError(x - 1, y + 1);
      addError(x, y + 1);
      addError(x + 1, y + 1);

      addError(x, y + 2);
    }
  }

  return img;
}

uint8_t* convertImage(uint8_t* image, int width, int height) {
  int bytesPerRow = (width + 7) / 8;
  int bitmapSize = bytesPerRow * height;

  uint8_t* bitmap = new uint8_t[bitmapSize];

  for (int y = 0; y < height; y++) {
    for (int byteX = 0; byteX < bytesPerRow; byteX++) {
      uint8_t b = 0;

      for (int bit = 0; bit < 8; bit++) {
        int x = byteX * 8 + bit;

        if (x < width) {
          int pixelIndex = y * width + x;
          uint8_t pixel = image[pixelIndex];

          // black pixel if grayscale value is dark
          if (pixel < 128) {
            b |= (1 << (7 - bit));
          }
        }
      }

      bitmap[y * bytesPerRow + byteX] = b;
    }
  }

  return bitmap;
}

void printImage(uint8_t* bitmap, int width, int height) {
  int bytesPerRow = (width + 7) / 8;

  printerSerial.write(0x1D);
  printerSerial.write(0x76);
  printerSerial.write(0x30);
  printerSerial.write(0x00);

  printerSerial.write(bytesPerRow & 0xFF);
  printerSerial.write((bytesPerRow >> 8) & 0xFF);

  printerSerial.write(height & 0xFF);
  printerSerial.write((height >> 8) & 0xFF);

  for (int i = 0; i < bytesPerRow * height; i++) {
    printerSerial.write(bitmap[i]);
  }

  printerSerial.write(0x0A);
}
