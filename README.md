# FrsCanulator
---------------
Canbus translation and emulation for the Toyobaru FRS using a Holley ECM.

* ABS Working
* EPS Working
* Gauges Working

Clears SRS malfunction indicator, flashes CEL with low oil pressure.

<br>
<i>Built using an Arduino R4 Minima with two MCP2515 modules, also tested on mega2560.
One mcp runs at 1mbps for the Holley canbus, and one runs at 500k for the vehicle canbus.
The holley data is read and transmitted to the vehicles canbus emulating the vehicles ECM messages to use the stock gauges.

Modules I've removed from the vehicle are emulated to keep the remaining modules in a normal state such as ABS and Power Steering. This is done by status counters and message frequency. Some counters use all 8 bits, some use the upper 4 bits and some use the lower 4 bits.
In the [86_Can_Info.md](https://github.com/0xBravoKilo/FrsCanulator/blob/main/86_Can_Info.md) file there is information about the message IDs, frequency each packet is transmitted and what some of the data in the packets represent.</i>
