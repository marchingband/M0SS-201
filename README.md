# M0SS-201 Source Code

This repository contains the complete source code for the M0SS-201, the mono west-coast style synth in the M0SS line. It runs on the Bouffalo Labs BL616, and is written in C. Unlike the 101, which uses [faust](https://faust.grame.fr/) compiled into C for it's core DSP code, the 201 is written directly in C.

## Structure

`dsp/synth.c` is the file that contains nearly all the code in the audio signal path. It generates samples one by one, taking in parameters and outputting a mono audio sample.

`dsp/interface.c` describes the button funcitons of the interface. Each parameter in the synth is assigned a button index, a type (main control, alt control, selector, etc), and a description for how it should be displayed.

`main.c` is the entry point of the program, it initializes the peripherals and starts their various FreeRTOS tasks.

`gpio.c` handles reading the buttons, and largely manages UI state.

## Help

For assistance in modifying this code, please contact us on our [discord](https://discord.gg/w7DQAhY46v)