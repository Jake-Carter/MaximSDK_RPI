## Description

A simple test program for checking the IN-PI33TBTPRPGPB programmable RGB LEDs on the MAX78000 PiHat board.

This version of the software will run a rainbow effect down the 4 on-board LEDs of the MAX78000 RPI Hat.

## Software

### Project Usage

Universal instructions on building, flashing, and debugging this project can be found in the **[MSDK User Guide](https://analog-devices-msdk.github.io/msdk/USERGUIDE/)**.

### Project-Specific Build Notes

* This project comes pre-configured for the MAX78000 EvKit.  See [Board Support Packages](https://analog-devices-msdk.github.io/msdk/USERGUIDE/#board-support-packages) in the UG for instructions on changing the target board.

## Setup

If using the MAX78000EVKIT (EvKit_V1):
-   Connect a USB cable between the PC and the CN1 (USB/PWR) connector.
-	Select "EvKit_V1" for _BOARD_ in "project.mk"
-   Connect Port 0, Pin 19 (P0.19) to the "Data In" line of the LED strip.
-   Connect the GND of the LED strip and MAX78000EVKit.
-   Define the LED Strip length in main.c

If using the MAX78000FTHR (FTHR_RevA)
-   Connect a USB cable between the PC and the CN1 (USB/PWR) connector.
-	Select "FTHR_RevA" for _BOARD_ in "project.mk"
-   Connect Port 0, Pin 19 (P0.19) to the "Data-In" line of the LED strip. 
-   Connect the GND of the LED strip and MAX78000FTHR.
-   Define the LED Strip length in main.c

If using the MAX78000 RPIHat (PIHAT_V1):
-   Connect a USB cable between the Raspberry Pi and the CN1 (USB/PWR) connector.
-   Open an terminal application on the Raspberry Pi and connect to the PiHat console UART at 115200, 8-N-1.
-	Select "PIHAT_V1" for _BOARD_ in "project.mk"

## Expected Output

You should observe a rainbow effect that travels down the LED strip.

The Console UART of the device will output these messages:

```
***** LED Strip Example *****
This example sends a rainbow animation through the onboard RGB LEDs.
LED Strip Length: 4 LEDs
LED Strip Resolution: 24 bits
Sitback and enjoy the show!
```
