This project aims to build a controller informing you when your plant needs watering. You will see the maximal and current moisture level and the time since the last watering. Finally, when the moisture level drops below the defined threshold, the alarm will rise - in the form of an LED flashing SOS signal.

<img src=/doc/img/overview.jpg width=300  />

I have another project measuring moisture based on capacitance and not resistance. This method is more precise and does not suffer corrosion. https://github.com/maciejmiklas/CapacitivePlantMonitor 

# Hardware
![](/fritzing/PlantMonitor_schem.jpg)

* you will find a fritzing project in the folder "fritzing."
* 2 PIN socket in the left bottom corner is meant to be connected to a moisture sensor
* R1 - regulates LCD brightness
* R2 - it's a photoresistor that regulates the insensitivity of LCD backlight
* R5 - regulates backlight insensitivity for LCD
* R6 - sets alarm threshold
* R10 + R14 - those two resistors are part of a voltage divider for the moisture sensor

# Software design
The whole project is divided into modules in a separate class files.

I did not use interrupts, so every module is controlled from the main loop in *PlantMonitor.cpp*.

Each module has to be initialized and has to execute some operations once in a while. Each module defines two functions for this proposal: *abc_setup()* and *abc_cycle()* - *abc* would be a module name. The setup method has to be called only once, and this happens in *PlantMonitor#setup()*. The cycle method will be called on every loop. Each cycle method internally measures time and runs only when it should (like every 100 ms) without blocking.

The method *Util#util_millis()* is used by each module to access the current time. The implementation of this method caches time during the execution of a single loop and updates its value at the beginning of each cycle. Using the same time during each cycle reduces the number of calls going to the timer. Each log statement within the same loop contains an identical timestamp, so we can logically connect events.

# Logger
The log module will produce messages over the COM port. It cannot be disabled because there is no reason to do so -  we do not have any other usage for the COM port, and sometimes it's helpful to see what is happening. 
A few variable resistors can be used to set up things like alarm thresholds. Those changes will also be printed over the COM port so that you can see how they affect functionality. Here is a log example:
```
>>[000-00:00:00,000]-> Initializing LCD module 
>>[000-00:00:00,000]-> Initializing hygrometer module
>>[000-00:00:00,038]-> Adopting LCD backlight. Sensor: 519, LCD: 177, Adjust(def 500): 605 
>>[000-00:00:00,038]-> Adjusted alarm sensitivity. 370 = 36% 
>>[000-00:00:01,000]-> Status -> Free RAM: 1270 
>>[000-00:00:12,000]-> Moisture has changed = 30%, Max: 30%, Status: 3 
>>[000-00:00:12,000]-> Recognised plant watering, status: 3 
>>[000-00:00:18,000]-> Reseting max proc based on adoption time after watering 
>>[000-00:00:18,000]-> Moisture has changed = 24%, Max: 24%, Status: 1 
>>[000-00:00:36,000]-> Reseting max proc based on adoption time after watering 
>>[000-00:00:36,000]-> Moisture has changed = 19%, Max: 19%, Status: 1 
>>[000-00:00:49,013]-> Adopting LCD backlight. Sensor: 559, LCD: 169, Adjust(def 500): 605 
>>[000-00:00:57,000]-> Reseting max proc based on adoption time after watering 
>>[000-00:00:59,169]-> Adopting LCD backlight. Sensor: 679, LCD: 145, Adjust(def 500): 605 
>>[000-00:01:12,139]-> Adopting LCD backlight. Sensor: 639, LCD: 153, Adjust(def 500): 604
```
# Moisture - Hardware
<img src=/doc/img/moistureSensor.jpg width=300  />

The sensor could be built from stainless steel nails or something similar. Depending on the material and distance between probes, they will have different resistance, so it needs to be calibrated.

On the schematic, you can see a connector called "MOISTURE SENSOR" - this one will be connected to your DIY sensor - just two wires that we will stick into the ground. The "MOISTURE SENSOR" and resistors R10+R14 are building a voltage divider. The voltage drop on R10+R14 will be provided to Arduino's analog input A0 - and this value will be transferred into moisture in percentage.

First, you have to determine the resistance of your moisture sensor. Stick it into the water and measure its resistance - multiply this value by two, which will be the value you should take for R10 - in my case, it was 20K. The resistor R14 is optional, and you can use it for fine-tuning. Assuming that R14 resistance is 0 and the sensor is in water, we will have a voltage drop on our sensor and twice the voltage drop on R10 - the value we measure on A0. Once we remove the sensor from the water, its resistance will increase, and the voltage drop on R10 will decrease.

Now it's time to calibrate the software - the moisture level shout be above 90% when the sensor is entirely in water. You have to adopt macro in MoistureMeter.h: *#define probeToPercent(read) (read/8)*  - header file contains further documentation.

# Moisture - Software (*Hygrometer.cpp*)
The method *hygro_sample(Moisture)* returns the current moisture level and status: "no change", "small change," and "level increased".
The method itself is called on every loop. Internally it executes only when needed. Otherwise, it returns "no change". 

Moisture measurement is executed every 5 minutes(*MESURE_FREQ_MS*). Only during this time, the moisture sensor is powered on. That is realized by a transistor connected to D8. Before we start taking probes, D8 goes low and powers up the sensor over the transistor (PNP). But we are not taking measurements immediately after powering on - there is a five-second warm-up period (*MOISTURE_WARM_UP_MS*). After the warm-up, we take ten probes (*PROC_PROBES*) every second (*MESURE_PROBE_WAIT_MS*). First, after collecting all ten probes, we calculate the median, which is the measured value.

The procedure takes several seconds, with different wait periods between particular steps. The implementation is based on state machine pattern - method *hygro_sample(Moisture)* will get called from the main loop every 100ms. It checks the current state, returns immediately if further delay is required, executes some logic, or moves to the next state. That means that we never block the main loop when some delay is necessary - implementation relies on the fact that it will get called every 100ms and so calculates the required delay.

It's worth mentioning that moisture change is being recognized with a tolerance of 5% (*MIN_TO_CHANGE*) - to avoid bouncing.

# LCD Display - Hardware
LCD is connected in a standard way, and variable resistor R1 can be used to adjust brightness. 

A photoresistor controls the insensitivity of LCD backlight. The photoresistor is connected to analog input A1 over a voltage divider.
The software calculates the insensitivity of the backlight based on reading from the photoresistor - this is a linear function. Additionally, variable resistor R5 connected to A2 is used to calculate the constant for our function: lower resistance increases measured voltage on A2 and increases backlight. It means that resistance change proportionally influences backlight for all light conditions (readings of photoresistor).
Resistance changes on R5 are being logged over COM so that you can see its influence on a backlight.

The LCD backlight is connected to the PWN output on D9 but does not go directly to the LCD input. Instead, it goes through an RC filter and op-amp. The RC filter uses a large capacitor, giving us smooth backlight insensitivity changes.
We would not need this whole RC filter because the PWM frequency is too high to observe flickering, and smoothness could be archived in software, but I want to solve this in hardware and have an excellent constant voltage.

You can see in the screenshots below the dependency between PWM duty (blue) and constant voltage (yellow). 
![](/doc/img/DS1Z_QuickPrint3.jpg)
![](/doc/img/DS1Z_QuickPrint4.jpg)

The time required to change the LCD backlight from maximal brightness to minimal takes 440 ms.
![](/doc/img/DS1Z_QuickPrint5.jpg)

# LCD Display - Software (*Lcd.cpp*)
The initialization phase (*lcd_stup()*) prints all static characters - those will not change anymore. All other characters will be only printed if they have changed - this reduces flickering. 
The lcd_cycle() will get called on every loop from *PlantMonitor#loop()*, and it adopts an LCD backlight for changing the environment.
*Lcd.cpp* also contains one function that prints time and another that publishes moisture.

# SOS LED
It's just a LED connected to A10. The *Sos.cpp* controls the blinking frequency. The *sos_setup()* is called only once and configures PIN A10 for output. *sos_cycle()* has to be called on every loop - it's a nonblocking method that will switch the LED on and off in SOS rhythm. You can configure all possible periods by changing corresponding variables in *Sos.h*. Two more methods play a significant role: sos_on() will enable the SOS signal, and respectively *sos_off()* will disable it. 

# Low moisture level alarm
When moisture gets below the defined level, the LED controlled by *Sos.cpp* will start flushing. It is being handled by *Alarm.cpp*. This module knows when the alarm has to be raised and when it stops. It also controls an alarm - currently, it's a LED signal, but you could connect something else. In this case, you have to only plug it in the right place in *Alarm.cpp*.
The alarm starts when moisture drops below the level defined by the variable resistor connected to PIN 3. The resistance will be mapped to percentage, and changes to this resistor are printed to the COM port - when you change the resistance, you can read the threshold for alarm.

# Getting up and running
Download the latest release and compile cpp files, *PlantMonitor.cpp* contains the main loop. 

Optionally you can directly upload compiled hex file:

```
sudo avrdude -patmega328p -carduino -P/dev/ttyUSB0 -b57600 -Uflash:w:PlantMonitor.hex:a
```
