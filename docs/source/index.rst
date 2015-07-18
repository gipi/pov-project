.. POV documentation master file, created by
   sphinx-quickstart on Sat Feb 28 12:23:53 2015.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to POV's documentation!
===============================

Principles
----------

Here we introduce the mathematics behind the calculation of the timing of the
led generated patterns: in order to calculate the timing we will using an
accelerometer; other systems(?) use a magnetic sensor to know the device
position and velocity but we don't like this solution since need to have a
magnet hanging on the bike itself.

In our solution we used the IC named ``MMA3202KEG``, that has two axis
with 100g and 50g of resolution. It's a linear accelerometer.

Let :math:`\omega` be the angular velocity of the wheel, and :math:`R` the
external radius of the wheel and :math:`r` the distance from the center of
the wheel and the accelerometer, we have that the velocity of the wheel is given by

.. math::

   V = \omega R

and the centrifugal acceleration is

.. math::

   a = \omega^2\cdot r

We need to know the angular velocity of the wheel in order to fit the timing
of the leds for the pattern to be displayed.

The problem is that the resulting vector (measured by the sensor) is given by the vectorial sum
of the usual gravity and the centrifugal force

.. math::

   \vec{a}_t = \vec{g} + \vec{a}_c

.. image:: vectors.png
   :align: center

Since from the sensor we obtain only the resulting vector we are missing also the
angle between :math:`\vec{a}_t` and :math:`\vec{g}`; to avoid the calculation at given time
we interpolate using the instants where the acceleration has its maximum and minimum:
in the first case the sensor is in the lowest position (so that gravity and centrifugal
acceleration sum up) and the last case is when the sensor is at the highest position.

We suppose that :math:`R=4.10^{-1}m/s` and the bike is running at :math:`V = 36 km/h = {3.6\cdot 10^{4}\over 3600} m/s=10 m/s`; the angular momentum
is given by

.. math::

   \omega = {10 m/s\over 4\cdot 10^{-1}m} = 25 s^{-1} 

so

.. math::

   a = \left(25\cdot s^{-1}\right)^2\cdot 4\cdot 10^{-1} m = 250 m/s^2 \approx 25g
