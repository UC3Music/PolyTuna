PolyTuna
-----

    This is a work in progress
    
A polyphonic tuner pedal. 

![Logo](doc/img/lots-of-tuna-fish.png)

### Dependencies

* pyAudio: `sudo apt-get install python-pyaudio python3-pyaudio`

#### Author
	
* Sergio Sánchez

#### Advisor
    
*  [David Estévez Fernández](https://github.com/David-Estevez)

### Teensy Edition Libraries information

* In order to get Polytuna fully working in Teensy, it's needed to overwrite these files with the ones included in the folder "Polytuna Teensy Edition":

	******TO AVOID FUTURE PROBLEMS, SAVE THE ORIGINAL FILES BEFORE OVERWRITING ANYTHING.******
	
	* "AudioStream.h":  
	
			* "(Arduino installation folder)\Arduino\hardware\teensy\avr\cores\teensy3" (Windows based systems)		 
			
			* "(Arduino installation folder)/Arduino/hardware/teensy/avr/cores/teensy3" (Linux based systems)
			
	* "output_i2s.cpp": 
	
			* "(Arduino installation folder)\Arduino\hardware\teensy\avr\libraries\Audio" (Windows based systems)	
			
			* "(Arduino installation folder)/Arduino/hardware/teensy/avr/libraries/Audio" (Linux based systems)
			
These files ensure the sampling frequency is 8kHz instead of the 44.1kHz standard in Teensy to get a more precise FFT.
