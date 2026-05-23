# DIY-Polaroid

This is a DIY polaroid using a thermal printer, an ESP32-CAM, and a battery! It allows you to take pictures and have them printed out, in black in white, in real time on receipt paper. 

As someone who loves to take photos and keeps lots of instant camera/photo booth photos at home, I excitedly bought a polaroid a couple years ago. However, due to the high cost of polaroid film, I have unfortunately only used it sparingly, unable to capture daily moments with my friends, pets, and family, as I had initially hoped for. With this thermal printer and camera duo, I will be able to take photos in a similar style of instant cameras, but without having to worry about wasting film or money! On Amazon, you can find thermal paper worth $0.7 per roll and on AliExpress, $0.33 per roll. 

This is also a fun way for me to learn more about hardware!

To use:
---
1. Slide bottom of case open, then slide battery adapter on to the battery
2. press the capture button at the top
3. wait for the photo to print out from the top!
4. to power off, open the bottom again and slide the battery off

How it works:
----
<img width="735" height="548" alt="image" src="https://github.com/user-attachments/assets/30ccb34a-b430-43ce-b10a-2ec7de5cec72" />


1. Pressing the shutter button sends a message to the ESP32-CAM to take a picture
2. Once a picture is taken, the ESP32 processes the image and converts it into a black-and-white bitmap suitable for thermal printing.
3. The processed image data is sent to the thermal printer through a serial (TTL) connection.
4. The thermal printer prints the image onto receipt paper in real time.
5. The entire system is powered by a Dewalt 12V battery connected to a buck converter, which safely steps the voltage down to 5V for the ESP32-CAM and printer.

Other notes:
- Copper wires will be used to connect battery to buck converter and will be screwed in
- Dupont wires will be used for the ESP32-CAM connections
- Components will be secured with electrical tape/hot glue if needed
- Power Reqs: 3.5 or 5v for ESP32-CAM and 12V for printer

CAD Details:
------
- The red button at the top is the only functional button. The others are part of the outer casing design.
<img width="424" height="337" alt="image" src="https://github.com/user-attachments/assets/6e2f4a3b-f86d-451c-b38f-438f7ebd9f53" />


- The bottom is a sliding bottom that, when pulled out, leaves room to slide in the battery into the adapter.


<img width="420" height="313" alt="image" src="https://github.com/user-attachments/assets/fb817163-8acf-4fe6-abf0-1de481f877e6" />

  
printer details:
<img width="745" height="503" alt="image" src="https://github.com/user-attachments/assets/956e3383-9656-4820-b4ba-8a2d3c577c38" />

  [source](https://www.aliexpress.us/item/2251801289844172.html?spm=a2g0o.productlist.main.7.57dd7c24dBbUDR&algo_pvid=06a76f2e-c004-4b5d-b338-92d0601850c3&algo_exp_id=06a76f2e-c004-4b5d-b338-92d0601850c3-6&pdp_ext_f=%7B%22order%22%3A%2277%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21USD%2122.67%2118.86%21%21%2122.67%2118.86%21%40210328d417778534163348069e3f68%2112000053272105761%21sea%21US%210%21ABX%211%210%21n_tag%3A-29910%3Bd%3A67c9d4b3%3Bm03_new_user%3A-29895%3BpisId%3A5000000204886261&curPageLogUid=BWBIKVt1n01r&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1476158924%7C_p_origin_prod%3A)
________________________________________________
<img width="457" height="654" alt="image" src="https://github.com/user-attachments/assets/e5c0b401-4615-44ad-9cd2-7227e43da462" />
_______________________________________________

Wiring:
<img width="678" height="326" alt="image" src="https://github.com/user-attachments/assets/b867340c-4df4-46f1-adc7-6e2598ed23b6" />

__________________________________________________

BOM:
| Item | Link | Cost |
--- | --- | --- |
| Battery |(https://www.aliexpress.us/item/3256811448662800.html) | $11.54 |
| ESP32-CAM + wires + FDTI | (https://www.amazon.com/Freenove-Dual-core-Microcontroller-Wireless-Projects/dp/B0CJJHXD1W)) | $6.58 | 
| Thermal Printer | (https://www.aliexpress.us/item/3256809632695801.html) | $19.69 |
| Button | (https://www.aliexpress.us/item/3256810419379908.html) | $0.99 | 
| Wire Cable (optional) | (https://www.amazon.com/dp/B09WHQ18KL) | $7.88 for a roll | 
| Battery Adapter | (https://www.amazon.com/dp/B07PPD7LWBr) | $6.99 | 
---------------
Total cost: $53.67

Cost of Polaroid: $149.99 (no film included) :(

Some sources for inspiration:
1. https://www.hackster.io/Mellow_labs/build-your-own-esp32-instant-camera-f12649
2. https://community.dfrobot.com/makelog-308490.html
