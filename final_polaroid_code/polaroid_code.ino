HardwareSerial printerSerial(1);
const int buttonPin = 7;
void setup() {
  pinMode(buttonPin, INPUT_PULLUP);

  printerSerial.begin(9600, SERIAL_8N1, -1, 5);
  delay(3000);
}

void loop() {
  if (ifButtonPressed()) {

    camera_fb_t* fb = takePhoto();
    if (fb == NULL) return;

    uint8_t* resized = resizeImage(fb);
    uint8_t* dithered = applyDithering(resized, 384, 288);
    uint8_t* bitmap = convertImage(dithered, 384, 288);

    printImage(bitmap, 384, 288);

    esp_camera_fb_return(fb);
    delete[] resized;
    delete[] bitmap;

    delay(1000);
  }
}
bool ifButtonPressed(){
  return digitalRead(buttonPin)== LOW;

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

    for(int y=0; y<height; y++) {

        for(int x=0; x<width; x++) {

            int i = y*width + x;

            int oldPixel = img[i];

            int newPixel;

            if(oldPixel < 128)
                newPixel = 0;
            else
                newPixel = 255;

            img[i] = newPixel;

            int error = oldPixel - newPixel;
            int spread = error/8;

            auto addError=[&](int xx,int yy){

                if(xx<0 || xx>=width ||
                   yy<0 || yy>=height)
                    return;

                int index = yy*width + xx;

                int value = img[index] + spread;

                if(value<0)
                    value=0;

                if(value>255)
                    value=255;

                img[index]=value;
            };

            addError(x+1,y);
            addError(x+2,y);

            addError(x-1,y+1);
            addError(x,y+1);
            addError(x+1,y+1);

            addError(x,y+2);
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

}