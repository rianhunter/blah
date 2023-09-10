# [CXADC][cxadc] clock generator and audio ADC

A project to synchronously clock two (or more) CXADC PCIe cards and an extra audio ADC from an external clock generator.
This enables sync capturing of two RF streams (e.g. RF Video and RF HiFi audio from a VHS), in addition to a stereo audio stream (e.g. linear VHS audio).

The project consists of 3 main components:
- A [PCB adapter][pcb-adapter-readme] that is installed per PCIe card.
  It contains only a couple of passive components per [cx25800-11z data sheet][google-cx25800-datasheet].
- The main PCB that combines a [Raspberry Pi Pico][pi-pico], with an [Adafruit Si5351A Clock Generator Breakout Board][si5351-adafruit], and a [PCM1802 board][ali-pcm1802-search].
- The firmware for the [Raspberry Pi Pico][pi-pico], that implements a USB sound card and also controls the [Si5351A][si5351-adafruit] clock generator.
  The firmware is available pre-build an can be flashed to the [Raspberry Pi Pico][pi-pico] without additional tool.

The two PCBs can be manufactured with [JLCPCB][jlcpcb] cheaply, and assembled easily with basic soldering tools.

## Installing

Compatible with the
- *VT610EX Rev:SC3A VTImage 19.07* capture cards, white pcb, featuring a *CX25800-11Z*

## Versioning

*TODO*

# License

The content of this repository is under various licenses.
The PCB parts are generally under *Creative Commons Attribution-ShareAlike 4.0*,
and the source code is under *TBD*.
For details see the individual *LICENSE.md* files in the respective folders they apply to.

[pcb-adapter-readme]: hardware/vt610ex-clock-generator-adapter/README.md
[cxadc]: https://github.com/happycube/cxadc-linux3/
[pi-pico]: https://www.raspberrypi.com/products/raspberry-pi-pico/
[si5351-adafruit]: https://www.adafruit.com/product/2045
[ali-pcm1802-search]: https://www.aliexpress.com/w/wholesale-PCM1802.html
[jlcpcb]: https://jlcpcb.com
[google-cx25800-datasheet]: https://www.google.com/search?q=DSH-201233A
