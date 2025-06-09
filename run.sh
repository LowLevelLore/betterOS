#!/usr/bin/env bash

set -e

# TAP device name
TAP="tun0"
BRIDGE="br0"

# Clean and build ISO
make clean
make betterKernel.iso

# Function to cleanup TAP on exit
cleanup() {
    echo "Cleaning up..."
    sudo ip link set $TAP down || true
    sudo ip link delete $TAP || true
}
trap cleanup EXIT

# Create TAP device if it doesn't exist
if ! ip link show "$TAP" &>/dev/null; then
    echo "Creating TAP device: $TAP"
    sudo ip tuntap add dev $TAP mode tap user $(whoami)
    sudo ip link set $TAP up
fi

# Attach TAP to bridge
sudo ip link set $TAP master $BRIDGE

# Run QEMU with TAP networking
sudo qemu-system-x86_64 \
    -machine pc \
    -cdrom build/betterKernel.iso \
    -boot d \
    -device pcnet,netdev=net0,mac=52:54:00:12:34:56 \
    -netdev tap,id=net0,ifname=$TAP,script=no,downscript=no \
    -vga std \
    -serial stdio \
    -drive id=disk,file=disk.img,format=raw,if=ide &

# Wait for VNC port and start viewer
sleep 2
vncviewer :5900
