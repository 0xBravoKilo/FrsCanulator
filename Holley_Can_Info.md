### Holley needs to be in racepak mode


Canbus IDs have the ecu serial as part of the ID. Filter out with `& 0x7FFFF000`

Values are 4 bytes and multiplied by 256.

<pre>
0x1E001000
byte 1-4	runtime msec
byte 5-8	rpm
</pre>

<pre>
0x1E021000
byte 5-8	cts
</pre>

<pre>
0x1E025000
byte 1-4	oilPr
</pre>

<pre>
0x1E01D000
byte 1-4	iat
byte 5-8	tps
</pre>

More information can be found in the NHRA EFI Spec publication.
<pre>https://www.nhraracer.com/Files/Tech/NHRA_EFI_Specifications_Rev10.pdf</pre>
