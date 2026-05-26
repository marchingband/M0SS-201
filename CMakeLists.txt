cmake_minimum_required(VERSION 3.15)

include(proj.conf)

find_package(bouffalo_sdk REQUIRED HINTS $ENV{BL_SDK_BASE})

sdk_add_include_directories(.)

target_sources(app PRIVATE
    midi.c
    midiXparser.c
    dsp.c
    usb.c
    rgb_led.c
    flash.c
    encoder.c
    rotary_encoder.c
    audac.c
    gpio.c
    spi_rgb.c
    dsp/interface.c
    dsp/synth.c
    dsp/biquad.c
)

sdk_set_main_file(main.c)

project(M0SS-201)
