# Kudzu

[TOC]

Compile:

```sh
pw build
```

Run the host build:

```sh
./out/gn/host_debug/obj/third_party/pigweed-experimental/applications/terminal_display/bin/terminal_demo
```

Flash a pico:

```sh
picotool load -f -x ./out/gn/rp2040/obj/third_party/pigweed-experimental/applications/terminal_display/bin/terminal_demo.elf
```
