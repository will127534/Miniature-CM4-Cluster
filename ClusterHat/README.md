# CM4-Cluster-Hat
The PCB board files for the main hat

## Notes
* The measurement of node's input current is on the CM4 Card (tps2420 efuse current monitor output)
* I tried to use GPIO pin straps to set up USB Hub, but there is no such configuration that works, so I end up placing an I2C EEPROM that contains the settings. There are three jumpers at the back of the board that connect that EEPROM to RPI & hold the reset of USB HUB, so you can/need to flash the EEPROM image the first time.
    * Also note that because reversing the polarity of USB data eases the layout, all of the device's USB polarity is swapped and configured as such, so that is why in the sch the P/N is reversed.
    * There might be some issue with the LED indicator on the USB Hub.
* You could connect the MII interface to RPI, but I'm not sure how to use it to turn it into a managed switch.
* And just to remind you that **the M.2 connector on this board is a non-standard pinout**, please don't attach anything other than the CM4 Card.
 
## Preview
![](https://i.imgur.com/ZVhvSxf.png)
![](https://i.imgur.com/XrFmksw.png)
