.. POV documentation master file, created by
   sphinx-quickstart on Sat Feb 28 12:23:53 2015.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to POV's documentation!
===============================

The project's aim is to develop a device that display, using the principles
of the **persistence of view**, a pattern on a spinning wheel (a bicycle one).

It utilizes an ``ATMega32U4`` as central processing unit and an accelerometer as
extenal sensor to calibrate internally the position and velocity of the device.

The geometry of the wheel is obviously circular, from `Wikipedia <https://en.wikipedia.org/wiki/Bicycle_wheel#Sizes>`_ we know that
the denomination of the wheels has the pattern::

    <width inflated tire>-<rim's inner width>

In our test we used one with designation ``22-622`` so that the radius of the wheel is 33mm

.. toctree::
   :maxdepth: 2

   wheel
   dynamics
   pattern
   electronics
   supply


