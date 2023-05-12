
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

#Add path's for .profile
echo "Creating GCC binary path in .profile"

#Add path's for .profile
echo "Creating GCC binary path in .profile"

sudo cat << EOF >> ~/.profile

# Set MAXIM_PATH to point to the MSDK
export MAXIM_PATH=~/MaximSDK

# Add Arm Embedded GCC to path (v10.3)
export ARM_GCC_ROOT=\$MAXIM_PATH/Tools/GNUTools/10.3
export PATH=\$ARM_GCC_ROOT/bin:\$PATH

# Add xPack RISC-V GCC to path (v10.2)
export XPACK_GCC_ROOT=\$MAXIM_PATH/Tools/xPack/riscv-none-embed-gcc/10.2.0-1.2
export PATH=\$XPACK_GCC_ROOT/bin:\$PATH

# Add OpenOCD to path
export OPENOCD_ROOT=\$MAXIM_PATH/Tools/OpenOCD
export PATH=\$OPENOCD_ROOT:\$PATH
EOF

echo "Setup of MAX78000 RPI Hat complete! Grats'"
