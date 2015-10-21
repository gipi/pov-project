This directory contains the code to burn in this board.

As bootloader you can use this

  https://github.com/sparkfun/32U4_Breakout_Board/tree/master/Bootloaders/CDC

In order to program with the bootloader you have to press ``RESET`` and quick
after that press the (not yet existent) ``BOOT`` button.

## Bootloader

There is an extracted tree from the Sparfun's Atmega32u4 breakout (original repository
is [here](https://github.com/sparkfun/32U4_Breakout_Board/tree/master/Bootloaders)).

First of all you have to set your fuse so that the ``HWBE`` is enabled and
also ``BOOT RESET``.

It's possible to build passing the path to the ``LUFA`` library path, like

    $ make LUFA_PATH=/opt/LUFA-130303/LUFA

(the command must be issued into the ``bootloader/CDC/`` directory). After
the ``hex`` file are generated you can upload with

    $ avrdude -c buspirate -p atmega32u4 -P /dev/ttyUSB0 -b 115200 -F -U flash:w:bootloader/CDC/BootloaderCDC.hex

After that the device will alternate on/off the first two leds of the board. The
bootloader is capable of emulating a ``avr109`` protocol and you can flash a file
with

    $ avrdude -c avr109 -p atmega32u4 -P /dev/ttyACM0 -F -U flash:w:path/to/file.hex
