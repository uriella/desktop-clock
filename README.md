# desktop-clock
a repository that contains some scripts for a microcontroller (especially ESPs) and lcd so then together, they can function as a clock.

The code is still messy. I'm planning on making some versions of it.
Currently, there's only the version.

This version relies on many libraries.

Let's discuss on the LCD first.
The LCD will be the medium for our desktop-clock information.
I use the LCD + I2C to make my life easier. 
With it, you only need 4 cables, one for the GND, VIN (pick the 5v), SDA (Serial Data), and SCL (Serial Clock).
In my case, the SDA pin can be found on the D21, and the SCL pin on D22.

The desktop-clock information comes from the Internet and to make life easier, I'm relying on some libraries.
You need to connect the microcontroller to the world.
I use the WiFi library and configured it with a static IP.
The WiFi confidential information can be found on the env.h file. (I don't know if this is the best practice or not)
After it is connected, it should work just fine.

In this version, I use DHT 11 module and make sure it is connected to a 3v input.
The DHT Sensor will captures humidity and temperature information.
In this code, the information will be stored as a json doc with the help of ArduinoJson.
Eventually, it will be sent to the serial port.

The DHT Sensor and ArduinoJson is actually optional. 
If you don't need it just comment the inclusion line.
I'm planning on sending the information to a Web Server.


P.S I only tested this code on ESP32, specifically the DOIT ESP32 DEVKIT V1.

