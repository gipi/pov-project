This directory contains the code to burn in this board as bootloader, it depends
on [LUFA](http://fourwalledcubicle.com/LUFA.php) version 140928.

In order to program with the bootloader you have to ``RESET`` the board while holding
the ``BOOT`` button.
After that the device will alternate on/off the first two leds of the board. 

## Bootloader

There is an extracted tree from the Sparfun's Atmega32u4 breakout (original repository
is [here](https://github.com/sparkfun/32U4_Breakout_Board/tree/master/Bootloaders)).

It's possible to build passing the path to the ``LUFA`` library path, like

    $ make LUFA_PATH=/opt/LUFA-140928/LUFA

(the command must be issued into the ``bootloader/CDC/`` directory). After
the ``hex`` file are generated you can upload with

    $ avrdude -c buspirate -p atmega32u4 -P /dev/ttyUSB0 -b 115200 -F -U flash:w:bootloader/CDC/BootloaderCDC.hex

The bootloader is capable of emulating a ``avr109`` protocol and you can flash a file
with

    $ avrdude -c avr109 -p atmega32u4 -P /dev/ttyACM0 -F -U flash:w:path/to/file.hex

It's possible to use the arduino IDE with the following ``boards.txt`` entries

```
ciaoduino.name=CiaoDuino
ciaoduino.vid.0=0x2341
ciaoduino.pid.0=0x0036
ciaoduino.vid.1=0x2341
ciaoduino.pid.1=0x8036
ciaoduino.vid.2=0x2A03
ciaoduino.pid.2=0x0036
ciaoduino.vid.3=0x2A03
ciaoduino.pid.3=0x8036

ciaoduino.upload.tool=avrdude
ciaoduino.upload.protocol=avr109
ciaoduino.upload.maximum_size=28672
ciaoduino.upload.maximum_data_size=2560
ciaoduino.upload.speed=57600
ciaoduino.upload.disable_flushing=true
ciaoduino.upload.use_1200bps_touch=true
ciaoduino.upload.wait_for_upload_port=true

ciaoduino.build.mcu=atmega32u4
ciaoduino.build.f_cpu=16000000L
ciaoduino.build.vid=0x2341
ciaoduino.build.pid=0x8036
ciaoduino.build.usb_product="Arduino Leonardo"
ciaoduino.build.board=AVR_LEONARDO
ciaoduino.build.core=arduino
ciaoduino.build.variant=leonardo
ciaoduino.build.extra_flags={build.usb_flags}
```

(we don't know what is strictly necessary) to flash directly into this board. Probably
a manual ``RESET`` is necessary.

## Fuses

You have to set your fuse so that the ``HWBE`` is enabled but not ``BOOT RESET``
(we want the bootloader to be loaded only with the boot button).

A working fuses configuration is obtained with the following command

    $ avrdude -c buspirate -p atmega32u4 -P /dev/ttyUSB0 -b 115200 -U lfuse:w:0xce:m -U hfuse:w:0x99:m -U efuse:w:0xc3:m

