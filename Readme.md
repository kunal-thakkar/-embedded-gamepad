## BLE wireless gamepad from existing NES Game controller

## Hardware Used
* Old NES Controller
* STM8S103 Microcontroller
* RN-42 BLE module

## Button Mappings
| Hex Code	|   Bit | X-Game    | SNES pad  | NES pad |
| :-------: | ----: | :-----    | :------   | :------ |
| 0x0100    |   1   |	A       | A         | A       |
| 0x0200	|   2   |	B       | B         | B       |
| 0x0400    |	3   |	C       |           | Select  |
| 0x0800    |	4   |	X       | X         | Start   |
| 0x1000    |	5   |	Y       |	Y       | UP      |
| 0x2000	|   6   |		    |   Z       | Down    |
| 0x4000    |	7   |Left Back  |	L1      | Left    |
| 0x8000    |	8   |Right Back |	R1      | Right   |
| 0x0001    |	9   |Back       |	L2      |         |
| 0x0002    |	10  |           |	R2      |         |
| 0x0004    |	12  |           |	Select  |         |
| 0x0008    |	13  | Start     | Start     |         |
| 0x0010    |	11  | Mode X    |           |         |
| 0x0020    |	14  | Left Stick Click| L3  |         |
| 0x0040    | 	15  | Right Stick Click| R3 |         |
| 0x0080    |	16  |           |           |         |
