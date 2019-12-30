### this Arduino library

wraps the HX711 library by bogde [https://github.com/bogde/HX711](https://github.com/bogde/HX711)

### this sensor

the HX711 is a 24-bit ADC with a gain amplifier (32, 64, 128) that is commonly used in strain guage and load cell applications

### quick links
 - purchase options
     -  [amazon](https://www.amazon.com/DIYmall-Weighing-Conversion-Sensors-Microcontroller/dp/B010FG9RXO)
 - arduino library [https://github.com/bogde/HX711](https://github.com/bogde/HX711)

### using this library
declare an hx711 instance and pass it to HalfBridgeLoadCell.
using setOnValueListener will run the callback whenever the read value is within errorThreshold
be careful when creating too many listener events on an Arduino, since it will take up a lot of clock cycles to loop through all of them
    #include <HX711.h>
    HX711 hx711;
    loadcell = HalfBridgeLoadCell(&hx711, dataPin, sckPin);
    loadcell.read();

    // to create an event listener, pass references to callback, valueToListen, errorThreshold
    void callback(float tiggerValue) {
        // do something
    }
    float valueToListen = 10.f;
    float errorThreshold = 1.f;
    loadcell.setOnValueListener(&callback, &valueToListen, &errorThreshold);

### future enhancements
* ability to delete listeners
* protothreading for listeners

### how to wire this sensor
HX711 usually has up to 10 pins

the first four connect to the arduino and are on one side of the board: VCC, GND, DT and SCK

wiring to the arduino is straightforward: **VCC** gets 2.7V-5V, **GND** to ground of arduino, **DT** to any digital port, **SCK** to any PWM enabled digital port.

the other six pins are on the opposite side: E+, E-, A+, A-, B+ and B-. These connect to one or more sensors based on your application.

#### four wire load cell
wiring to a *four wire* load cell: connect VCC (red wire) to **E+**, GND (black wire) to **E-**, SIGNAL ACTIVE (white wire) to **A+** and SIGNAL REFERENCE (green wire) to **A-**. [link to helpful instructable](https://www.instructables.com/id/How-to-Interface-HX711-Balance-Module-With-Load-Ce/)

#### three wire load cell
wiring to a *three wire* load cell is a bit more tricky, you must create a half or full-wheatstone bridge circuit

For half-bridge, treat one load cell as Active and the other as Reference. [helpful stackexchange answer](https://electronics.stackexchange.com/a/199907)
- SIGNAL *REFERENCE* (white wire) and GND *ACTIVE* (black wire) to **E+**
- SIGNAL *ACTIVE* (white wire) and GND *REFERENCE* (black wire) to **E-**
- VCC *ACTIVE* (red wire) to **A-**
- VCC *REFERENCE* (red wire) to **A+**


### 3rd party library being used
the HX711 arduino library being used is [https://github.com/bogde/HX711](https://github.com/bogde/HX711). Refer to its [README](https://github.com/bogde/HX711/blob/master/README.md) to learn more about it.

Steps to get started with the library being wrapped:

 1. declare an instance of the HX711 class
 2. `HX711::begin(dtPin, sckPin)`
	 * dtPin and sckPin are digital pins. sckPin has to support PWM
 3. calibrate the sensor
    `HX711::set_scale(scaleOffset)`
	`HX711:tare()` 
 4. to get readings in a non-blocking manner, use `HX711::wait_ready_timeout(timeMillis)` where timeMillis is frequency in milliseconds. `wait_ready_timeout` returns a boolean. If it's true, call `HX711::get_units(numOfReadings)` to get an average reading of numOfReadings.
