# Arduino EEPROM programmer
 Program a AT28C256P with an Arduino Uno and a couple of SN74HC595N Shift Register ICs
## Wiring
![Arduino EEPROM Programmer Schematic](https://github.com/user-attachments/assets/4357a42c-eaa6-497b-863d-a2148017b4af)

This is how I wired the Arduino up with the shift registers and the EEPROM chip. It looks very confusing but most of that is just the data and address buses, which follow a very logical pin mapping. I've explained these in writing which is probably easier to understand than the schematic:
 * Pins D9-D2 of the Arduino are connected to pins IO7-IO0 of the EEPROM
 * Pins QA-QH of the first shift register (U2) are connected to pins A0-A7 of the EEPROM
 * Pins QA-QG of the second shift register (U3) are connected to pins A8-A14 of the EEPROM

Once you've wired those up, the rest is fairly simple. Just follow the schematic to fill in the gaps.

## Code
code.ino contains all you need to program the EEPROM. So far, it is only possible to define the programmed data by defining a byte array within the program itself. I am working on a version that automatically reads and uploads a .bin file when the arduino gets flashed.

