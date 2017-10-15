Hardware implementation of the Polytuna pedal. The schematics can be found in the [Thesis document](https://github.com/UC3Music/PolyTuna/raw/master/Memoria.pdf). Schematics will be ported to some EDA software (probably Kicad) in the future.

This firmware requires both [Arduino](https://www.arduino.cc/) and the [Teensyduino add-on](https://www.pjrc.com/teensy/teensyduino.html).

In order to get Polytuna fully working in Teensy, some stock libraries need to be modified. These changes ensure the sampling frequency is 8kHz instead of the 44.1kHz standard in Teensy to get a more precise FFT.

    **TO AVOID FUTURE PROBLEMS, SAVE THE ORIGINAL FILES BEFORE OVERWRITING ANYTHING.**
	
* `AudioStream.h`. Overwrite the following file: `(Arduino installation folder)/Arduino/hardware/teensy/avr/cores/teensy3/AudioStream.h` (with '\' instead of '/' on Windows-based systems).
			
* `output_i2s.cpp`. Overwrite the following file: `(Arduino installation folder)/Arduino/hardware/teensy/avr/libraries/Audio/output_i2s.cpp` (with '\' instead of '/' on Windows-based systems).
