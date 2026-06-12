# Aqara camera IFTTT interface

The Aqara Camera Hub G3 offers AI-driven features such as face or pet detection. Unfortunately, these events cannot be used as triggers in IFTTT (at least in Europe). This project provides a workaround by making use of the camera's built-in IR functionality.

After setting up an automation in the Aqara Home app, the camera sends an IR signal when an AI event occurs (e.g. a specified face is detected). The IR sensor of this device receives the signal, and the ESP32 calls an IFTTT webhook in response, effectively bridging the Aqara ecosystem with any IFTTT-connected service.

> **Prerequisites:** Aqara Home App, IFTTT pro account

## How it works

The Aqara Home app allows the camera to imitate a TV remote control and send infrared signals. This project uses a virtual LG TV remote, where each number button (0–9) corresponds to a different AI event. The IR codes for these buttons use command values 16–25 (an LG-specific encoding), which the ESP32 decodes back to 0–9 before constructing the IFTTT webhook URL.

This allows up to 10 different AI events to trigger separate IFTTT applets.

## Setup

To use the interface you need the Aqara Home App and an IFTTT pro account to enable webhooks.

### Virtual TV remote control in Aqara Home App
1. Open the Aqara Home app and navigate to your camera.
2. Tap **Infrared Remote Control** at the bottom of the camera interface.
3. Add a new remote control and select **TV → LG**. LG is required as each brand uses unique IR codes and this project is built around LG's encoding.
4. Confirm the connection test and follow the on-screen instructions.

### Infrared Remote Control Sensor
Create a plain text file named `wifi.txt` on a MicroSD card with the following format:

```
WiFi-name,WiFi-password,IFTTT-event-prefix,IFTTT-key
```

| Field | Description |
|---|---|
| `WiFi-name` | Your WiFi network name (SSID) |
| `WiFi-password` | Your WiFi password |
| `IFTTT-event-prefix` | A prefix for your IFTTT event names, e.g. `my_trigger_` |
| `IFTTT-key` | Your IFTTT Webhook API key (found at `https://maker.ifttt.com/use/YOUR_KEY_HERE`) |

**Example:**
```
MyNetwork,secret123,my_trigger_,abc123xyz
```

### Camera AI automations in Aqara Home

1. Go to **Automation** in the Aqara Home app and tap the **+** button.
2. Select your camera under **Accessories** as the trigger (**When**) and choose an AI event (e.g. *Specified face detected*).
3. As the action (**Then**), select the virtual LG remote and assign a button between **0 and 9**.

#### IFTTT setup

1. Create a new applet with **Webhooks** ("Receive a web request") as the trigger.
2. Name the event using your chosen prefix followed by a number, e.g. `my_trigger_1`.
3. Choose any service as the output (**Then**).

## List of parts

| Name | Description |
|---|---|
| Wemos D1 Mini ESP32 | Microcontroller board |
| MicroSD Card Module | SPI-based MicroSD card reader |
| MicroSD Card (≤32GB) | Stores the credentials file |
| VS1838B / OS-1838B IR Receiver | Infrared signal sensor |
| PCB | Carrier board for all modules |
| Connectors | For connecting modules to the PCB |
| USB cable | Powers the microcontroller |

## Carrier board for all modules

### Circuit

![Circuit of the carrier board for the modules](/assets/images/circuit.png)

### Handmade PCB

![Handmade PCB](/assets/images/pcb.png)

## Todo

- [ ] Improve code
    - [ ] Become independent of specifically simulating the IR signals of an LG TV (IR Address 4 and number buttons 16–25 instead of 0–9)
- [ ] Create PCB for production

## License

This project is licensed under the GPL-3.0 License.
© Till Maria Jürgens / MOVEN Research Group, Rhine-Waal University of Applied Sciences