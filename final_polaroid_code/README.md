
!The code here is untested and includes placeholders that will be updated once I can experiment and test with the real printer 

This folder will contain the main usable firmware for the DIY Polaroid camera. 

## Overall code flow:
1. ESP32-CAM powers on
2. Camera is initialized using the camera pin configuration
3. The thermal printer serial connection is initialized
4. Theshutter button pin is set as an input
5. The code waits for the user to press the shutter button
6. When the button is pressed, the ESP32-CAM captures a photo in grayscale
7. The photo is converted into a format that can be processed for printing
8. The image is resized to fit the width of the thermal printer paper (57.5±0.5mm)
9. The image is converted into black and white pixels
10. Dithering is applied
11. The processed image is converted into bitmap data
12. The bitmap data is sent to the thermal printer though serial communication
13. The printer prints the photo onto the receipt paper
14. The code returns to waiting for another button press

The shutter button will be connected to one of the ESP32-CAM GPIO pins
When button is pressed, ESP32-CAM will capture a frame

general file structure:

final_polaroid_code/

-final_polaroid_code.ino

-camera_setup.h

-printer_setup.h

-image_processing.h
