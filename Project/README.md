# Project
## User Guide  
This project is basically a hall effect keyboard prototype with additional sensors, a display and a servo strapped onto.
I wanted to do this project due to the fact that hall effect switches have several large advantages over the popular mechanical designs and convinced my group partner Fabian to choose this as our project.
However, the keyboard portion by itself was deemed to be too simple, hence the added sensors, display and servo.

The project works by using the keys to switch between several output modes that display the information provided by the sensors in various ways.  
Page 0 shows the outputs of all sensors as a value,  
Page 1 shows the adc value of switch 1,  
Page 2 shows the adc value of switch 2 as a bar,  
Page 3 shows the adc value of switch 3 by moving the servo accordingly,  
Pages 4, 5 and 6 show the temperature in °C, °F and K respectively,  
Page 7 shows the measured output voltage of the temperature sensor,  
Page 8 shows the echo time of the ultrasound sensor,  
Page 9 shows the measured distance in cm using the textbook speed of sound,  
Pages 10 and 11 show the measured distance in cm and in respectively using the measured temperature to calculate the actual speed of sound.

Basically everything about the keyboard is designed to be configurable using the options in config.h.
The amount of keys can be defined using the ROWS and COLUMNS options. The trigger height can be set individually for each switch, in a range from 0.05 to 4 mm.
Each switch can also be disabled by setting its trigger height to 0. If the readings vary too much between the switches, a calibration function
can be configured. It saves the values to EEPROM and therefore only needs to be ran when a variance is detected.
Checks to make sure that the parameters are within spec are also in place, telling you exactly what the problem is and how to fix it.
A rapid trigger function is also thrown in just for fun, but it works perfectly well.

## Link to zip file
https://drive.google.com/file/d/1n6c50Sl8Roaz2_w2AhfxIroRF0-IB2Su/view?usp=drive_link

## Link to demonstration video
https://youtu.be/dFBEIjEhHFQ  
[![Demonstration Video](https://img.youtube.com/vi/dFBEIjEhHFQ/hqdefault.jpg)](https://www.youtube.com/watch?v=dFBEIjEhHFQ "Demonstration Video")

## Wiring Diagram
![Wiring Diagram](https://github.com/vermilion00/PSoC-Course/blob/master/Project/images/circuit.png "Wiring Diagram")

## Parts List
### PSoC 5LP CY8C5888LTI-LP097 x1  
https://www.infineon.com/dgdl/Infineon-PSoC_5LP_CY8C58LP_Family_Datasheet_Programmable_System-on-Chip_(PSoC_)-DataSheet-v15_00-EN.pdf?fileId=8ac78c8c7d0d8da4017d0ec547013ab9  
Can be bought directly from Infineon or through many reputable sites.

### 49E Hall Effect Sensor x16 (Or however many keys you want)  
https://www.diodes.com/assets/Datasheets/AH49E.pdf  
I ordered these from Aliexpress, so no information on the supplier, but no issues so far.

### GEON Raptor HE switches x16 (Or however many keys you want)  
No data sheet available  
I ordered these from Aliexpress, so no information on the supplier, but no issues so far.

### SSD1306 OLED display x1  
https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf  
I ordered this from Aliexpress a long while ago, so no information on the supplier. The first ~40 lines have issues.

### HC-SR04 x1  
https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf  
Can be bought from many different vendors like SparkFun.

### Reely S-0008 x1  
https://asset.conrad.com/media10/add/160267/c1/-/en/002148502DS00/datenblatt-2148502-reely-mini-servo-analog-servo-getriebe-material-plastik-stecksystem-jr.pdf  
Can be bought from Conrad and other vendors.

### LM35DZ  
https://www.ti.com/lit/ds/symlink/lm35.pdf  
Can be bought from most electronics vendors like Mouser or Reichelt.