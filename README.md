PolyTuna
-----

    This is a work in progress
    
A DIY polyphonic guitar tuner pedal. With it you can check the tuning of all the strings with a single strum, saving you time when you need it most. Very useful for playing live!

![Logo](doc/img/lots-of-tuna-fish.png)

This work has been developed by [Sergio Sánchez](https://github.com/SerjSanchez) as his Bachelor's Thesis, with [David Estevez](https://github.com/David-Estevez) as his advisor. The Thesis itself can be downloaded [here](https://github.com/UC3Music/PolyTuna/raw/master/Memoria.pdf) (in Spanish).

The work has three main parts:

* A preeliminary study of the guitar signal to decide the best analysis technique for the pedal. This study was made in Matlab and the sources for the study can be found in the `MATLAB Analysis` folder.
* A software implementation of the polyphonic tuner to validate the algorithms prior to the hardware implementation. This code is written in Python and contained in the `Software tuner` folder.
* A hardware implementation (i.e. the pedal itself). The firmware for the device is stored in the `Polytuna Teensy Edition` folder. The schematics can be found in the [Thesis document](https://github.com/UC3Music/PolyTuna/raw/master/Memoria.pdf). The full circuit and the PCB KiCad project can be found at [`Polytuna KiCad`](https://github.com/UC3Music/PolyTuna/tree/master/Polytuna%20KiCad) folder.

Some pictures of the prototype:
<img src="doc/img/polytuna.jpg" alt="pic1">
<img src="doc/img/polytuna_02.jpg" width="433x" alt="pic2">
<img src="doc/img/polytuna_03.jpg" width="433px" alt="pic3">
<img src="doc/img/polytuna_04.jpg" width="170px" alt="pic4">
<img src="doc/img/polytuna_05.jpg" width="170px" alt="pic5">
<img src="doc/img/polytuna_06.jpg" width="170px" alt="pic6">
<img src="doc/img/polytuna_07.jpg" width="170px" alt="pic7">
<img src="doc/img/polytuna_08.jpg" width="170px" alt="pic8">

#### Author
	
* Sergio Sánchez

#### Advisor
    
*  [David Estévez Fernández](https://github.com/David-Estevez)
