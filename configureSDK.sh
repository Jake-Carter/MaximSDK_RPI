
#!/bin/bash

#Auto install Script
echo ""
echo "Starting install process for Maxim Integrated MAX78000 SDK on RPI"
echo ""

ARCHITECTURE=$(arch)
echo ""
echo "Linux Architecture: "$ARCHITECTURE
echo ""

#Install Linux Packages for USB
echo ""
echo "Installing Linux USB Packages for USB Debugging"
echo ""
 
sudo apt install --yes libhidapi-hidraw0 libhidapi-dev libncurses5-dev libncurses5



#Copy rules file to /etc/udev/rules.d
echo "Copying Permissions File to /etc/udev/rules.d/"
sudo cp OpenOCD/60-openocd.rules /etc/udev/rules.d/
echo ""

#refresh
sudo udevadm control --reload-rules
sudo udevadm trigger --attr-match=subsystem=net

echo "Setup of MAX78000 RPI Hat complete! Grats'"
