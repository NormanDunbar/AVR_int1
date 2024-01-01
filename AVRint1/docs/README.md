# AVR_int1 Library

The `AVR_int1` library provides a simple interface to the INT1 interrupt on an Arduino running with an ATmega328P and *probably* works on an ATmega168 as well -- they are extremely similar.

The code has been tested on projects created in the Arduino IDE version 1.8.18 onwards including 2.2.1, and also in PlatformIO (platformio.org), with no problems on either system.

## Arduino

To install the library:

* Click Sketch->Include Library->Add .ZIP Library;
* Navigate to where the zip file for this library is located on your system;
* Select it and click the OK button;
* Follow the prompts.


## PlatformIO

As of the time of typing, I have not built this library into a format suitable for uploading to the official PlatformIO Library Registry.

* Unzip the downloaded zip file.
* In your project's directory, create a new directory under `lib`, named `AVR_int1`.
* Copy files from the `src` directory *of the extracted zip file* to thie new directory you created above.
* That's it. You only need to `#include` the appropriate header file in your code, and the library will be used without requiring any `lib_deps` settings. Easy!

Norman Dunbar

15 September 2020.
