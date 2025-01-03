### Module and corresponding message IDs with transmission frequency.

<pre>
ECM
0x140 10ms
0x141 10ms
0x142 10ms
0x144 20ms
0x360 50ms
0x361 50ms
</pre>

<pre>
ABS
0xD0  20ms
0xD1  20ms
0xD2  20ms
0xD3  20ms
0xD4  20ms
</pre>

<pre>
SRS
0x372 100ms
0x4DD 1000ms
</pre>

<pre>
GAUGES
0x156 20ms
0x282 60ms
0x442 380ms
</pre>

<pre>
STEERING ANGLE
0x18  10ms
0x4C6 1000ms
</pre>

<pre>
AC
0x280 20ms
0x46C 380ms
0x4DC 1000ms
</pre>

<pre>
BCM
0x440 380ms
0x152 20ms
0x374 1000ms
0x375 1000ms
0x63B 500ms
0x6E1 1000ms
0x6E2 1000ms
</pre>

<pre>
CERT
0x284 100ms
0x44D 380ms
0x37A 1000ms
</pre>

<pre>
PS ECU
0x370 50ms
0x4C8 1000ms
</pre>

<pre>
UNKNOWN
0x3D1 120ms
</pre>

<br>

### ABS Data Identification

<pre>
0xD0    LEN: 8    Data: 94 FB FF FF 00 00 FF FF
Data[0-1] steering angle: 0x0000-7FFF straight to right, 0xFFFF-8000 straight to left
Data[2-7] accelerometer?
</pre>

<pre>
0xD1    LEN: 4    Data: 00 00 29 00
Data[0-1] speed
Data[2] brake pressure
</pre>

<pre>
0xD3    LEN: 7    Data: 20 06 C0 0F 00 00 FF
Data[0-1] traction control mode: 0x0006-Normal, 0x080E-VSC button pushed, 0x2006-TC button pushed, 0x200E-TC button held
Data[2] 0xC0 brake not pressed, 0xE0 brake is pressed
Data[4] counts 0x00-FF while moving
Data[6] status counter 0x00-FF
</pre>

<pre>
0xD4  	LEN: 8    Data: 5F 02 5D 02 5E 02 5D 02
Data[0-1] LF wheel speed
Data[2-3] RF wheel speed
Data[4-5] LR wheel speed
Data[6-7] RR wheel speed
</pre>

<br>

### SRS Data Identification

<pre>
0x372    LEN: 8    Data: 00 B0 00 00 00 00 00 00
Data[1] status counter +16 (0x00-F0)
Data[2] 0x00-normal (airbag indicator off)
</pre>

<br>

### ECM Data Identification

<pre>
0x140    LEN: 8    Data: 00 14 69 43 00 00 0A 01
Data[0] accel pedal position
Data[1] first 4 bits count 00-0F, clutch at bit 8 (0x80), 90, 10, 00 seen
Data[2-3] rpm: bytes are reversed, 0x6A46 = 1642, 0x6846 = 1640 masking off 0x4000 (bit 14 goes high when accel pedal is 0 🤷‍♂️)
Data[4-5] dup of byte 1, delayed
Data[6] throttle body position
Data[7] in idle? 01 for idle, 00 for rev, 21 idle warm
</pre>

<pre>
0x141    LEN: 8    Data: 8A 26 32 2B 47 87 27 00
Data[4-5] rpm for tach: 0x8000-A300 = 0-8960rpm, bytes are reversed [5]-msb
</pre>

<pre>
0x144    LEN: 8    Data: C0 00 4C 17 94 A4 A0 00
Data[2] oil temperature unverified
Data[3] coolant temperature unverified
</pre>

<pre>
0x360    LEN: 8    Data: 5A 08 39 39 93 00 01 00
Data[3] temp gauge: 0x58 reads C, 0x80-94 normal, 0x94-A3 reads to H
Data[5] 0x10-Green Cruise Light, 0x20-Green Set Light, 0x30-Green Set & Cruise Lights.
</pre>

<pre>
0x361    LEN: 8    Data: 00 09 00 D3 04 E0 AA 81
Data[1] 0x20-Check Engine Light off, 0x40-flash 4hz, 0x60-flash 0.5hz, 0x80-flash 1hz, 0xA0 flash 2hz
Data[4] 0x80-Yellow Cruise Light
</pre>

<br>

### AC Data Identification

<pre>
0x280  	 LEN: 8    Data: 00 03 00 00 00 00 00 00
Data[0] status of buttons, defog,fanspd...
Data[1] status counter 0x00-0F
</pre>

<br>

### BCM Data Identification

<pre>
0x374    LEN: 8    Data: 08 00 00 01 00 00 00 00
Data[3] driver door open
Data[1] status counter 0x00-0F
</pre>

<pre>
0x375    LEN: 8    Data: 00 01 00 00 00 00 00 10
Data[1] driver door open
Data[2] lock/unlock button
Data[3] headlights
</pre>

<br>

### Steering Angle Data Identification

<pre>
0x18     LEN: 8    Data: 00 02 70 08 00 00 00 9A
Data[0-1] steering angle: 0x0000-7FFF straight to left, 0xFFFF-8000 straight to right
Data[7] steering velocity?
</pre>

<br>

### PS ECU Data Identification

<pre>
0x370    LEN: 8    Data: 67 00 00 00 00 03 00 00
Data[0] torque
Data[5] status counter 0x00-0F
</pre>
