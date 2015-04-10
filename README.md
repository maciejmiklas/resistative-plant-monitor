The goal of this project is to build a controller that will inform you when your plant needs watering. You will see the maximal and current moisture level and the time passed since last watering. Finally when moisture level drops below defined threshold the alarm will rise - in form of LED flashing SOS signal.

<img src=/doc/img/overview.jpg width=300  />

# Hardware
![](/fritzing/PlantMonitor_schem.jpg)

* you will find fritzing project in folder "fritzing"
* 2 PIN socket in the left bottom corner is meant to be connected to moisture sensor
* R1 - regulates LCD brightness
* R2 - it's a photo resistor which regulates insensitivity of LCD backlight
* R5 - regulates backlight insensitivity for LCD display
* R6 - sets alarm threshold
* R10 + R14 - those two rezistors are part of a voltage divider for moisture sensor

# Software design
Whole project is divided into modules, each one in separate class file.

I dis not use interrupts, so every module is controlled from main loop in *PlantMonitor.cpp*.

Each module has to be initialized and has to execute some operations once in a while. For this propose each module defines two functions: *abc_setup()* and *abc_cycle()* - *abc* would be a module name. The setup method has to be called only once and this happens in *PlantMonitor#setup()*. The cycle method will be called on every loop. Each cycle method internally measures time and runs only when it should (like every 100 ms) without blocking.

The method *Util#util_millis()* is used by each module to access current time. The implementation of this method internally caches time during execution of single loop and updates it's value at the beginning of each cycle. Using the same time during each cycle not only reduces amount of calls going to the timer, also each log statement within the same loop contains identical time, so we can logically connect events.

# Logger
The log module will produce messages over COM port. It cannot be disabled because ... there is no reason to do so -  we do not have any other usage for COM port and sometimes it's useful to see what is happening. 
There are a few variable resistors, that can be used to set up things like alarm threshold. Those changes will be also printed over COM port, so that you can actually see how they effect functionality. Here is a log example:
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

The sensor itself could be build from stainless steel nails, or something similar. Depending on the material and distance between probes it will have different resistance and therefore it needs to be calibrated.

On the schematic you can see connector called "MOISTURE SENSOR" - this one will be connected to your DIY sensor - those are just two wires that we will basically stick into a ground. The "MOISTURE SENSOR" and resistors R10+R14 are building a voltage divider. The voltage drop on R10+R14 will be provided to Arduino's analog input A0 - and this value will be transferred into moisture in percentage.

First you have to determine resistance of your moisture sensor. Stick it into water and measure it's resistance - multiply this value by 2 and this will be the value that you should take for R10 - in my case it was 20K. The resistor R14 is optional and you can use it for fine tuning. Assuming that R14 resistance is 0 and sensor is in water, we will have voltage drop on our sensor and twice the voltage drop on R10 - and this is the value that we are measuring on A0. Once we start removing sensor from water, it resistance will increase and voltage drop on R10 will respectively decrease.

Now it's time to calibrate the software - moisture level shout be above 90% when the sensor if fully in water. You have to adopt macro in MoistureMeter.h: *#define probeToPercent(read) (read/8)*  - header file contains further documentation.

# Moisture - Software (*Hygrometer.cpp*)
The method *hygro_sample(Moisture)* returns current moisture level and status: "no change", "small change" and "level increased".
The method itself is being called on every loop, internally it executes only when it's needed, otherwise it returns "no change". 

Moisture measurement is being executed every 5 minutes(*MESURE_FREQ_MS*). Only during this time the moisture sensor is powered on. This is realized by transistor connected to D8. Before we start taking probes, D8 goes low and powers up sensor over transistor (PNP). But we are not taking measurements immediately after powering on - there is five second warm up period (*MOISTURE_WARM_UP_MS*). After the warm up is over, we are taking 10 probes (*PROC_PROBES*), each one every second (*MESURE_PROBE_WAIT_MS*). First after collecting all of 10 probes we calculate median and this is finally the measured value.

The whole procedure takes several seconds and there are different wait periods between particular steps. The implementation is based on state machine pattern - method *hygro_sample(Moisture)* will get called from main loop every 100ms, it checks current state, returns immediately if further delay is required, or executes some logic, or just moves to next state. This means that we never block main loop when some delay is required - implementation relays on fact, that it will get called every 100ms and so calculates required delay.

It's worth mentioning, that moisture change is being recognized with tolerance of 5% (*MIN_TO_CHANGE*) - just to avoid bouncing.

# LCD Display - Hardware
LCD display is connected in a standard way, variable resistor R1 can be used to adjust brightness. 

The insensitivity of LCD backlight is controlled by photo resistor. Photo resistor is connected to analog input A1 over voltage divider - this is a common pattern used to archive stable readings that are independent from current changes.
Software calculates insensitivity of backlight based on reading from photo resistor - this is a linear function. Additionally variable resistor R5 connected to A2 is used to calculate constant for our function: lower resistance increases measured voltage on A2 and increases backlight. This means, that resistance change proportionally influences backlight for all light conditions (readings of photo resistor).
Resistance changes on R5 are logged over COM, so you can exactly see it's influence on backlight.

The LCD backlight is connected to PWN output on D9, but it does not go directly to LCD input, instead it goes trough RC filter and op-amp. The RC filter uses large capacitor, this gives us smooth changes of backlight insensitivity.
Actually we would not need this whole RC filter, because PWM frequency it to high to observe flickering and smoothness could be archived in software, but I just wanted to solve this in hardware and have nice constant voltage.

You can see on screenshots below dependency between PWM duty (blue) and constant voltage (yellow). 
![](/doc/img/DS1Z_QuickPrint3.jpg)
![](/doc/img/DS1Z_QuickPrint4.jpg)

The time required  to change LCD backlight from maximal brightness to minimal takes 440 ms.
![](/doc/img/DS1Z_QuickPrint5.jpg)

# LCD Display - Software (*Lcd.cpp*)
The initialization phase (*lcd_stup()*) prints all static characters - those will not change any more,  all other characters will be printed only if they have changed - this reduces flickering. 
The lcd_cycle() will get called on every loop from *PlantMonitor#loop()* and it adopts LCD backlight for changing enviorment.
*Lcd.cpp* contains also one function that prints time and another function that prints moisture - that's all.

# SOS LED
t's just a LED connected to A10. The Sos.cpp controls the blinking frequency. The sos_setup() is called only once and it configures PIN A10 for output. sos_cycle() has to be called on every loop - it's non blocking method and it will switch the LED on and off in SOS rhythm. You can configure all possible time periods by changing corresponding variables in Sos.h. There are two more methods, that play significant role: sos_on() will enable SOS signal and respectively sos_off() will disable it. The cycle method have to be called on every cycle, even when SOS is disabled - in this case it returns immediately.

# Low moisture level alarm
When moisture gets below defined level the LED controlled by *Sos.cpp* will start flushing. This is being controlled by *Alarm.cpp*. This module knows when the alarm has to be raised and when it stops, it also controls kind of alarm - currently it's a LED signal, but you could connect something else. In this case you have to only plug it on the right place in *Alarm.cpp*.
Alarm starts when moisture drops below level defined by variable resistor connected to PIN 3. The resistance will be mapped to percentage and changes to this resistor are printed to COM port - when you change the resistance you can read the threshold for alarm.

# Getting up and running
Download latest release and compile cpp files, *PlantMonitor.cpp* contains main loop. I've used Eclipse, but importing this project makes no sense, it never works....

Optionally you can directly upload compiled hex file:

```
sudo avrdude -patmega328p -carduino -P/dev/ttyUSB0 -b57600 -Uflash:w:PlantMonitor.hex:a
```

Follow this receipt if you would like to import project into Eclipse:

1. setup Eclipse with Arduino support
2. create "New Arduino sketch" and name it PlantMonitor
3. import into your project LiquidCristal lib
4. clone git project into temp location
5. copy all *.cpp* and *.h* files from temp location into folder containing your Eclipse project. Copy operation should replace  *PlantMonitor.cpp*  and *PlantMonitor.h*.
