# Three-Body (Photon) Decay Simulation Program

## Description

This is a simple decay program that can generate three gamma decay from positronium.

![Alt text](info/splash.png "sim splash")
*Simulated Event: green events are collected in our detector*

## Compile

In order to compile this program you need the following library:

* ROOT 6
* Armadillo C++

then you can compile it using:


`g++ -I include/ test_det.cc src/detector.cc src/photon.cc src/event.cc src/run_manager.cc src/exp_setup.cc  ```root-config --cflags --glibs``` -std=c++11 -larmadillo -o sim`


## Usage

You can specify the number of events to be generated as follow:
'''
./sim *number_of_events_to_be_generated*
'''
