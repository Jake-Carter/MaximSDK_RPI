## Description

Demonstrates flash wear leveling on MAX78000 MCU.
 
The internal storage flash memory block count is specified by *FLASH_STORAGE_PAGE_CNT* macro.
 
 ```
 #define FLASH_STORAGE_PAGE_CNT 8
 ```
 
 that corresponds to 64kb (8 of 8kb blocks)

## Software

### Project Usage

Universal instructions on building, flashing, and debugging this project can be found in the **[MSDK User Guide](https://analog-devices-msdk.github.io/msdk/USERGUIDE/)**.

### Project-Specific Build Notes

* This project comes pre-configured for the MAX78000EVKIT.  See [Board Support Packages](https://analog-devices-msdk.github.io/msdk/USERGUIDE/#board-support-packages) in the UG for instructions on changing the target board.

## Required Connections
If using the MAX78000EVKIT (EvKit_V1):
-   Connect a USB cable between the PC and the CN1 (USB/PWR) connector.
-   Connect pins 1 and 2 (P0_1) of the JH1 (UART 0 EN) header.
-   Open a terminal application on the PC and connect to the EV kit's console UART at 115200, 8-N-1.

If using the MAX7800FTHR (FTHR_RevA):
-   Connect a USB cable between the PC and the CN1 (USB/PWR) connector.
-   Open a terminal application on the PC and connect to the EV kit's console UART at 115200, 8-N-1.

If using the MAX78000 RPIHat (PIHAT_V1):
-   Connect a USB cable between the Raspberry Pi and the CN1 (USB/PWR) connector.
-   Open an terminal application on the Raspberry Pi and connect to the PiHat console UART at 115200, 8-N-1.

## Expected Output

The Console UART of the device will output these messages:

```
***** MAX78000 Wear Leveling *****
Filesystem is mounted
boot_count: 12

Example Succeeded

```

