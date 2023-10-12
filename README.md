# Kudzu

[TOC]

## Compile:

```sh
pw build
```

## Run:

### Host

Run the host app and connect to it via `pw-system-console`:

```sh
./out/gn/host_device_simulator.speed_optimized/obj/applications/badge/bin/badge & \
  pw-system-console --socket-addr default \
    --proto-globs third_party/pigweed/pw_rpc/echo.proto ; \
  killall badge
```

### Kudzu

```sh
export ELF=./out/gn/rp2040.size_optimized/obj/applications/badge/bin/badge.elf

picotool reboot -f -u && \
  sleep 3 && \
  picotool load -x $ELF
```

Connect with `pw-system-console`:

```sh
pw-system-console --verbose \
  --baudrate 115200 \
  --proto-globs third_party/pigweed/pw_rpc/echo.proto \
  --token-databases ./out/gn/rp2040.size_optimized/obj/applications/badge/bin/badge.elf \
  --device /dev/rp2040
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
