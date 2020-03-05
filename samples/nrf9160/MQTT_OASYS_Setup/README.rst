A script for setting up the LTE-M modem with the MQTT broker.

The code runs in a loopback in order to establish a Client-Server connection.

We then have the possibility to send commands and recive status, data, enable diving mode etc.
In this example, if we publish "d" to the nRF9160, the code will break out of the loopback and can start executing tasks. (Just edit to whatever command you want)

See the Prj CONF file for MQTT topics, MQTT broker and Other configurations.


 
Troubleshooting
===============

Public MQTT brokers might be unstable.
If you experience problems connecting to the MQTT broker, try switching to another broker by changing the value of the ``MQTT_BROKER_HOSTNAME`` configuration option.


Dependencies
************

This sample uses the following libraries:

From |NCS|
  * ``drivers/lte_link_control``

From nrfxlib
  * :ref:`nrfxlib:bsdlib`

From Zephyr
  * :ref:`MQTT <zephyr:networking_reference>`

In addition, it uses the following samples:

From |NCS|
  * :ref:`secure_partition_manager`

