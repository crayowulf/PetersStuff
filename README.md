# PetersStuff

The lighting directory contains the code run on the Arduino.  The program contains several lighting functions
and a method for reading from the Serial port with minimal delay.  The program is simply loaded onto the Arduino
from the Arduino IDE and controled by a C program.

The directory writeToArduinoSerial contains the controlling program for the Arduino lighting system.  It contains
a makefile so simpley running "make" compiles the file.  The program reads in the system information from a text
file and sends the neccicary parts to the Arduino on its Serial port. After executing the program several options
are avaiable.
Typing 1 gives a rainbow flowing across the LEDs; 
  2 gives a changing single color flow across the LEDs;
  3 displays the tempeasures of the system, CPU temps and GPU temps are shown on four LEDs each, each node's
    temperatures are displayed between each gap on the top of the system and repeated along the supports;
  4 displays the node usage information, however there is an error in TEGRASTATS that reads each usage as 0%, so 
    this will not display any usefull information;
  5 displays a fade between RGB, and CMY;
  6 deisplys an improved fade between all the colors in 5, without going to black inbetween.
  
The directory siec includes a client and server for running an SIEC encrypted command shell.  To compile run the
instruction "make arch=ARM64 CC=gcc", for other systems replace the architecture and c compiler with those needed
there.

The directory sysInfo contains scripts for obtaining the temperature and usage information from the cluster.  The
system will not run rsh instruction from a script so perBoard.sh needs to be run on each node.  One node then runs
shell.sh to gather all of the individual information and save it in one file.
