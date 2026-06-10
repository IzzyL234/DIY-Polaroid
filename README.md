# DIY-Polaroid
This is a DIY polaroid using a thermal printer, an ESP32-CAM, and a battery! It allows you to take pictures and have them printed out, in black in white, in real time on receipt paper. 

As someone who loves to take photos and keeps lots of instant camera/photo booth photos at home, I excitedly bought a polaroid a couple years ago. However, due to the high cost of polaroid film, I have unfortunately only used it sparingly, unable to capture daily moments with my friends, pets, and family, as I had initially hoped for. With this thermal printer and camera duo, I will be able to take photos in a similar style of instant cameras, but without having to worry about wasting film or money! On Amazon, you can find thermal paper worth $0.7 per roll and on AliExpress, $0.33 per roll. 

This is also a fun way for me to learn more about hardware!

<img width="705" height="1000" alt="image" src="https://github.com/user-attachments/assets/05294e94-e150-495c-bc32-4ebaff136cee" />
<br/>

<span style="color: #2ECC71;">How it Works:</span>
----
Diagram 1:
<br/><br/>
<img width="735" height="548" alt="image" src="https://github.com/user-attachments/assets/30ccb34a-b430-43ce-b10a-2ec7de5cec72" />


1. Pressing the shutter button sends a message to the ESP32-CAM to take a picture
2. Once a picture is taken, the ESP32 processes the image and converts it into a black-and-white bitmap suitable for thermal printing.
3. The processed image data is sent to the thermal printer through a serial (TTL) connection.
4. The thermal printer prints the image onto receipt paper in real time.
5. The entire system is powered by a Dewalt 12V battery connected to a buck converter, which safely steps the voltage down to 5V for the ESP32-CAM and printer.

Other notes:
- Copper wires will be used to connect battery to buck converter and will be screwed in
- Dupont wires will be used for the ESP32-CAM connections
- Power Reqs: 3.5 or 5v for ESP32-CAM and 12V for printer

Other Details:
------
[STEP files and Full Assembly](Design_Submission_Materials)
  
- printer details:

<img width="372.5" height="251.5" alt="image" src="https://github.com/user-attachments/assets/956e3383-9656-4820-b4ba-8a2d3c577c38" />

__________________________________________________
## Assembly Instructions!

### 1. Print the enclosure parts
3D print the main enclosure pieces:

- `top.step`
- `bottom_box.step`
- `outside_box.step`


### 2. Prepare the electronics
Gather the main components:

- ESP32-CAM
- Thermal printer
- Shutter button
- Battery or power adapter
- Wires/jumper cables

### 3. Fit the components inside

Note: The button will be placed through the hole at the top and is the only functional button. The others are part of the outer casing design. The printer rests on a platform with areas for the wires to go through. The buck converter and ESP32 CAM will be secured to the sides with snap pieces. The top will stay on by itself, it is secured with a fitted lid.
<br/><br/>

<img width="647" height="402" alt="image" src="https://github.com/user-attachments/assets/c716ac47-dac1-4b57-a76d-36720dea1652" />
<br/><br/>
Side View:
<img width="260" height="255" alt="image" src="https://github.com/user-attachments/assets/a06651fa-afb2-4f8f-a89c-5f0f50d8fea6" />
<br/><br/>
-Place the ESP32-CAM inside the enclosure so the camera lens lines up with the front opening. It should snap into place
-Place the buck converter inside the enclosure under the ESP32-CAM. It should snap into place. 
-Slide the battery adapter onto the battery and slide it into the bottom section of the enclosure. The wire should go up through the cut out on the side.
-Place the thermal printer on top of the platform so that the wires from the bottom fit through the cutout on the platform. 
-Wires should snap into place on the sides. 
-Connect the button to the wire and loop the wire through the hole in the cap. 

See diagram above for visual details. 



### 6. Wiring
Connect the ESP32-CAM to the thermal printer using serial communication.

Wiring: see wiring diagram below
<img width="754" height="359" alt="image" src="https://github.com/user-attachments/assets/81aa8298-50e9-4600-a6af-9986487fd086" />


## Firmware Instructions
### 1. Upload the file "polaroid code" into Arduino as a sketch and upload to the ESP32
Double check that the pins noted in the code match what you actually have wired.

To use:
---
1. Press and hold button on top for 2 sec. 
2. press the capture button at the top once
3. wait for the photo to print out from the top!
4. to power off, open the bottom again and slide the battery off

BOM:
---
| Item | Link | Cost |
--- | --- | --- |
| Battery |(https://www.aliexpress.us/item/3256811448662800.html) | $11.54 |
| ESP32-CAM + wires + FDTI | (https://www.amazon.com/Freenove-Dual-core-Microcontroller-Wireless-Projects/dp/B0CJJHXD1W)) | $6.58 | 
| Thermal Printer (film roll included) | (https://www.aliexpress.us/item/3256809632695801.html) | $19.69 |
| Button | (https://www.amazon.com/dp/B07PPD7LWB) | $6.99 | 
| Battery Adapter | (https://www.amazon.com/dp/B09N42RJKW) | $16.90 | 
---------------
Total cost: $61.70

Cost of Polaroid: $149.99 (no film included) :(

Some sources for inspiration:
1. https://www.hackster.io/Mellow_labs/build-your-own-esp32-instant-camera-f12649
2. https://community.dfrobot.com/makelog-308490.html
