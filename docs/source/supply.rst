Power Supply
============

There are two main ways to furnish current to the board

 #. USB: it's intended only for programming
 #. a battery connected to the connector ``JP6``: it's the standard way

Probably will be used a LiPo battery with a voltage of 3.7 volts that
will be connected to a boost converter by the ``JP6`` connector;
the core of the DC-DC converter is the ``TPS6104x`` IC.

.. math::

   I_P = 400\hbox{mA} + {V_I\over L} \cdot 100 \hbox{ns}

.. math::

   f_{S_{max}} = {V_{I_{min}}\cdot\left(V_O - V_I\right)\over I_P\cdot L \cdot V_O}

