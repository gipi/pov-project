This directory contains the code to burn in this board.

As bootloader you can use this

  https://github.com/sparkfun/32U4_Breakout_Board/tree/master/Bootloaders/CDC

In order to program with the bootloader you have to press ``RESET`` and quick
after that press the (not yet existent) ``BOOT`` button.

## Bootloader

There is an extracted tree from the Sparfun's Atmega32u4 breakout (original repository
is [here](https://github.com/sparkfun/32U4_Breakout_Board/tree/master/Bootloaders)).

It's possible to build passing the path to the ``LUFA`` library path, like

    $ make LUFA_PATH=/opt/LUFA-130303/LUFA
