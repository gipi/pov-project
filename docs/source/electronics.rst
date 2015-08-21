Electronics
===========

Microcontroller
---------------

We are using a ``ATMega32U4`` that has 16MHz clock and a timer with 8-bits resolution
and two available ADC channels with 10 bits resolution.

Accelerometer
-------------

In our solution we used the IC named ``MMA3202KEG``, that has two axis
with 100g and 50g of resolution. It's a linear accelerometer.

Soldering
---------

 #. ATmega32U4
 #. ICPS header
 #. the crystal and the related capacitors
 #. the diode connecyting ISP header with IC
 #. test the programmer sees the device
 #. reset button and reset resistor
 #. the passive part near the USB connector
 #. the USB connector
 #. solder the bridge to supply power to the IC
 #. test if the bootloader is accessible
 #. leds
 #. accelerometer
