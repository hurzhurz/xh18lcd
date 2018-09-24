This is a very early version of a XH18-LCD firmware made for the TSDZ2 Flexible OpenSource firmware.

**Use it at your own risk!**

To build it, follow the [instructions of the example](../README.md).  
And differing from that:
* Use the reduced UTFT version: UTFT.zip
* If needed, add sdcc options at sduino's platform.txt: "--opt-code-size --max-allocs-per-node 10000"
