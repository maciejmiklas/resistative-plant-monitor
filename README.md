The goal of this project is to build a controller that will inform you when your plant needs watering. You will see the maximal and current moisture level and the time passed since last watering. Finally when moisture level drops below defined threshold the alarm will rise - in form of LED flashing SOS signal.
![Overview](/doc/img/overview.jpg)

# Hardware
![Overview](/fritzing/PlantMonitor_schem.jpg)

* you will find fritzing project in folder "fritzing"
* 2 PIN socket in the left bottom corner is meant to be connected to moisture sensor
* R1 - regulates LCD brightness
* R2 - it's a photo resistor which regulates insensitivity of LCD backlight
* R5 - regulates backlight insensitivity for LCD display
* R6 - sets alarm threshold
* R10 + R14 - those two rezistors are part of a voltage divider for moisture sensor

# Software design
I've used Eclipse. Whole project is divided into modules, each one in separate class file.

I dis not use interrupts, so every module is controlled from main loop in PlantMonitor.cpp.

Each module has to be initialized and has to execute some operations once in a while. For this propose each module defines two functions: abc_setup() and abc_cycle() - abc would be a module name. The setup method has to be called only once and this happens in PlantMonitor#setup(). The cycle method will be called on every loop. Each cycle method internally measures time and runs only when it should (like every 100ms) without blocking.

The method Util#util_millis() is used by each module to measure time. This method caches current time and returns this cached value - internally it obtains time at the beginning of each cycle.
Using the same time during each cycle not only reduces amount of calls going to the timer, also each log statement within the same loop contains identical time, so we can logically connect events.

# Logger
The log module will produce messages over COM port. It cannot be disabled because ... there is no reason to do so -  we do not have any other usage for COM port and sometimes it's useful to see what is happening. 
There are a few variable resistors, that can be used to set up things like alarm threshold. Those changes will be also printed over COM port, so that you can actually see how they effct functionality. Here is a log example:

>>[000-00:00:00,000]-> Initializing LCD module >>[000-00:00:00,000]-> Initializing hygrometer module
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

# Moisture - Hardware
![Moisture Sensor](/fritzing/moistureSensor.jpg) TODO
The sensor itself could be build from stainless steel nails, or something similar. Depending on the material and distance between probes it will have different resistance, and therefore it needs to be calibrated. 

On the schematic you can see connector called "MOISTURE SENSOR" - this one will be connected to your DIY sensor - those are just two wires that we will stick into a ground. The "MOISTURE SENSOR" and resistors R10+R14 are building a voltage divider. The voltage drop on R10+R14 will be provided to Arduino's analog input A0 - and this value will be transfered into moisture in percentage. 

Stick your moisture sensor into watter and measure it's resistance - multiply this value by 2 and this will be the value that you should take for R10 - in my case it was 20K. The resistor R14 is optional and you can use it for fine tuning. Assuming that R14 resistance is 0, and sensor is in watter, we will have voltage drop on this sensor, and twice voltage drop on R10 - and this is the value that we are measuring on A0. Once we start removing sensor from watter, it resistance will increase and voltage drop on R10 will respectively decrease.

XXXXX

# Moisture - Software (Hygrometer.cpp)
The method hygro_sample(Moisture) returns current moisture level and status: "no change", "small change" and "level increased".
hygro_sample(Moisture) is being called on every loop, internally it executes only every 100ms, otherwise it returns "no change". With each run (every 100ms) it probes moisture level, but it does not return it immediately, it stores it in internal array and returns "no change". First after collecting 30 probes, it finds the median and returns proper status. It's worth mentioning, that moisture change is being recognized with tolerance of 5% - just to avoid bouncing.

# LCD Display - Hardware
LCD display is connected in a standard way, variable resistor R1 can be used to adjust brightness. 

The insensitivity of LCD backlight is controlled by photo resistor. Photo resistor is connected to analog input A1 over voltage divider - this is a common pattern used to archive stable readings that are independent from current changes.
Software calculates insensitivity of backlight based on reading from photo resistor - this is a linear function. Additionally variable resistor R5 connected to A2 is used to calculate constant for our function: lower resistance increases measured voltage on A2 and increases backlight. This means, that resistance change proportionally influences backlight for all light conditions (readings of photo resistor).
Resistance changes on R5 are logged over COM, so you can exactly see it's influence on backlight.

The LCD backlight is connected to PWN output on D9, but it does not go directly to LCD input, instead it goes trough RC filter and op-amp. The RC filter uses large capacitor, this gives us smooth changes of backlight insensitivity.
Actually we would not need this whole RC filter, because PWM frequency it to high to observe flickering and smoothness could be archived in software, but I just wanted to solve this in hardware and have nice constant voltage.

You can see on screenshots below dependency between PWM duty (blue) and constant voltage (yellow). 
![Rigol](/doc/img/DS1Z_QuickPrint3.jpg)
![Rigol](/doc/img/DS1Z_QuickPrint4.jpg)

The time required  to change LCD backlight from maximal brightness to minimal takes 440 ms.
![Rigol](/doc/img/DS1Z_QuickPrint5.jpg)

# LCD Display - Software (Lcd.cpp)
The initialization phase (lcd_stup()) prints all static characters - those will not change any more,  all other characters will be printed only if they have changed - this reduces flickering. 
The lcd_cycle() will get called on every loop from PlantMonitor#loop() and it adopts LCD backlight for changing enviorment.
Lcd.cpp contains also one function that prints time and another function that prints moisture - that's all.

# SOS LED
t's just a LED connected to A10. The Sos.cpp controls the blinking frequency. The sos_setup() is called only once and it configures PIN A10 for output. sos_cycle() has to be called on every loop - it's non blocking method and it will switch the LED on and off in SOS rhythm. You can configure all possible time periods by changing corresponding variables in Sos.h. There are two more methods, that play significant role: sos_on() will enable SOS signal and respectively sos_off() will disable it. The cycle method have to be called on every cycle, even when SOS is disabled - in this case it returns immediately.

# Low moisture level alarm
When moisture gets below defined level the LED controlled by Sos.cpp will start flushing. This is being controlled by Alarm.cpp. This module knows when the alarm has to be raised and when it stops, it also controls kind of alarm - currently it's a LED signal, but you could connect something else. In this case you have to only plug it on the right place in Alarm.cpp.
Alarm starts when moisture drops below level defined by variable resistor connected to PIN 3. The resistance will be mapped to percentage and changes to this resistor are printed to COM port - when you change the resistance you can read the threshold for alarm.

# Getting up and running
Download latest release from here and compile cpp files, PlantMonitor.cpp contains main loop. I've used Eclipse, but importing this project makes no sense, because it contain hardcoded paths.

Optionally you can directly upload compiled hex file:

sudo avrdude -patmega328p -carduino -P/dev/ttyUSB0 -b57600 -Uflash:w:PlantMonitor.hex:a
