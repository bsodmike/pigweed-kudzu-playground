# Kudzu

[TOC]

## Compile:

```sh
pw build
```

## Run:

### Host Build

Run either of the host build apps:

```sh
./out/gn/host_debug/obj/applications/terminal_display/bin/terminal_demo
```

```sh
./out/gn/host_debug/obj/applications/32blit_demo/bin/32blit_demo
```

### Flash a RP2040:

```sh
picotool reboot -f -u
sleep 3
picotool load -x ./out/gn/rp2040/obj/applications/terminal_display/terminal_demo.uf2
```

```sh
picotool reboot -f -u
sleep 3
picotool load -x ./out/gn/rp2040/obj/applications/32blit_demo/32blit_demo.uf2
```

## Linux Setup

### Udev Rules:

Put the following into `/usr/lib/udev/rules.d/49-picoprobe.rules`

```
# Pico app mode
SUBSYSTEMS=="usb", ATTRS{idVendor}=="2e8a", ATTRS{idProduct}=="000a", MODE:="0666"
KERNEL=="ttyACM*", ATTRS{idVendor}=="2e8a", ATTRS{idProduct}=="000a", MODE:="0666", SYMLINK+="rp2040"

# RP2 Boot
SUBSYSTEMS=="usb", ATTRS{idVendor}=="2e8a", ATTRS{idProduct}=="0003", MODE:="0666"
KERNEL=="ttyACM*", ATTRS{idVendor}=="2e8a", ATTRS{idProduct}=="0003", MODE:="0666", SYMLINK+="rp2040"

# Picoprobe
SUBSYSTEMS=="usb", ATTRS{idVendor}=="2e8a", ATTRS{idProduct}=="0004", MODE:="0666"
KERNEL=="ttyACM*", ATTRS{idVendor}=="2e8a", ATTRS{idProduct}=="0004", MODE:="0666", SYMLINK+="picoprobe"
```

This will also symlink `/dev/picoprobe` and `/dev/rp2040` to the respective
vendor and product ids.

Apply the above rules with:

```sh
sudo udevadm control --reload-rules
sudo udevadm trigger
```
