# Software
Pico Software and USB hub EEPROM image

## Pico Software

The code is a slightly modified version of the adc_console.c from the Pico C/C++ SDK, the main changes are adding more commend to control the GPIO pins and reading current measurement.  
```
Selecting the channel/node, send c + [node number]
Enabling the power (Pull EN pin on the CM4 high), send e
Disabling the power (Pull EN pin on the CM4 low), send d
Asserting USB boot pin, send U
Disasserting USB boot pin, send u
Resetting the CM4 via RUN pin (holding it low for 100ms), send r 
Reading the ADC raw and voltage measurement, send s
Reading all the CM4's current, send a
```

## USB hub configuring

Apart from the name customization, the USB pins are configured to be flipped for all the downstream ports that are used.

To flash the binary, you can use the following command after you connect the EEPROM to RPI.
```

```