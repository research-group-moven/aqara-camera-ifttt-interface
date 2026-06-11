# Aqara camera IFTTT interface

The Aqara Camera Hub G3 offers AI driven features such as face or pet detection. Unfortunately these triggers cannot be setup (at least in Europe) in IFTTT to create an applet. The present device allows to bypass the usual way making use of the IR functionality of the camera. After setting up an automation in the Aqara Home app, the camera sends an IR signal triggered by an AI event (e.g. known face detected). The IR sensor of the device recieves the signal and the ESP32 calls the webhook of IFTTT.

## Setup

Coming soon.

### Virtual TV remote control in Aqara Home App
The camera can imitate a TV remote control and send infrared signals. In this way we can make use of the number buttons of the remote control and up to 10 camera AI events can trigger other events via IFTTT. To set up the remote control you navigate to the camera in the Aqara Home app and select the button Infrared Remote Control in the bottom of the camera interface. On the bottom you can add a new remote control. Select under TV the brand LG as every company uses unique codes and we picked LG as the interface for this setup. The app asks for a test of the connection, you just have to confirm everything and follow the instructions.

### Infrared Remote Control Sensor
Write a text file wifi.txt on the SD card with credentials in the following pattern:

``WiFi name,WiFi-password,IFTTT-event,IFTTT-key``

Set up your personal WiFi name and password. Add the IFTTT API key of the webhook that you find under Settings of the webhook service. Use only the cryptic key after ``[...]/use/…``. Instead of ``IFTTT-event`` you can choose any name (e.g., ``my_trigger_``) that is used later as the name of the trigger followed by a number between 0 and 9.

### Camera AI events

#### Aqara Home app

After all devices are set up in the Aqara Home app you can create automations with the plus sign in the section Automation. You can select the camera under Accessories as a trigger (When) and a specific event (e.g., Specified face detected). As an action (Then) you can select the virtual remote control and add a remote control button between 0 and 9. 

#### IFTTT

According to the number you have to set up an applet with webhook (“Receive a web request”) as trigger (If) and name the event as following: “[IFTTT-event][0 – 9]”, e.g., “my_trigger_1”. More specifically: The IFTTT-event name you previously set up on the SD card and followed by a number from 0 to 9 is attached to the name, referring to the buttons on the virtual remote control. This allows for 10 different triggers by the AI functionality of the camera. Finally you can choose any service as output (Then).


## List of parts

Name | Description
---|---
Wemos D1 Mini ESP32 | Microcontroller Board
MicroSD Card Module | MicroSD card read and write
MicroSD Card 32GB | Micro SD card to store the credentials
OS-1838B IR receiver | Sensor to receive infrared signals
PCB | PCB as platform for all modules
Several connectors for PCB | Connecting modules with PCB

## Circuit

Coming soon.

## Todo

- [ ] Create and add PCB
- [ ] Add circuit
- [ ] Improve code
    - [ ] Become independent of specifically simulating the IR signals of an LG TV (IR Address 4 and number buttons 16–25 instead of 0–9)

## License

This project is licensed under the GPL-3.0 License.
© Till Maria Jürgens / MOVEN Research Group, Rhine-Waal University of Applied Sciences