## An Arduino library to use the Motorshield

* Support for DC motors (2 per chield)
* Support for stepper motors (1x4-wire per shield)

![Image of motorshield board](/motorshield.png?raw=true)

You can add this to your Arduino IDE following
[this guide](https://www.arduino.cc/en/guide/libraries)

You can build the example by entering the examples folder and run
`make -k`. This requires the Arduino build environment, the Debian
package can be installed with `sudo apt-get install arduino-mk`.

The example can run DC motors in manual or auto mode as well as
stepper motors in manual and auto mode. You can configure this in the
example.
