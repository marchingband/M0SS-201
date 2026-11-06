# M0SS-201 Source Code

This repository contains the complete source code for the M0SS-201, the mono west-coast style synth in the M0SS line. It runs on the Bouffalo Labs BL616, and is written in C. Unlike the 101, which uses [faust](https://faust.grame.fr/) compiled into C for it's core DSP code, the 201 is written directly in C.

## Build firmware (macOS)

Install Git, Make, CMake 3.x (3.19.3 or newer), and the macOS Xuantie RISC-V toolchain (`Xuantie-900-gcc-elf-newlib-v2.8.1`). Add the toolchain's `bin` directory to your `PATH` so `riscv64-unknown-elf-gcc` is available. See the [SDK's toolchain setup notes](https://github.com/bouffalolab/bouffalo_sdk/tree/9a267ff0f9c40fce3efcd4c92a726349381a9b31#readme) for installation guidance.

Clone the SDK at the exact commit used by this project, then clone this repository inside it:

```sh
git clone https://github.com/bouffalolab/bouffalo_sdk.git
cd bouffalo_sdk
git checkout 9a267ff0f9c40fce3efcd4c92a726349381a9b31
git clone https://github.com/marchingband/M0SS-201.git
git apply M0SS-201/bouffalo_sdk.patch
cd M0SS-201
make CHIP=bl616 BOARD=bl616dk
```

[`bouffalo_sdk.patch`](bouffalo_sdk.patch) contains the local SDK board, macOS build-tool selection, and bundled CherryUSB changes. CherryUSB and FreeRTOS are included in the pinned SDK; no separate clones are needed. The patch selects macOS tools.

Edit the firmware sources and run `make` again. Build output is in `build/build_out/`.

## Structure

`dsp/synth.c` is the file that contains nearly all the code in the audio signal path. It generates samples one by one, taking in parameters and outputting a mono audio sample.

`dsp/interface.c` describes the button funcitons of the interface. Each parameter in the synth is assigned a button index, a type (main control, alt control, selector, etc), and a description for how it should be displayed.

`main.c` is the entry point of the program, it initializes the peripherals and starts their various FreeRTOS tasks.

`gpio.c` handles reading the buttons, and largely manages UI state.

## Help

For assistance in modifying this code, please contact us on our [discord](https://discord.gg/w7DQAhY46v)
