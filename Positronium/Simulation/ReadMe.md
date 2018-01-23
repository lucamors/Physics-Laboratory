# Three-Body (Photon) Decay Simulation Program

## Description

This is a simple decay simulation program that can generate three gamma decay from positronium.

![Alt text](info/splash.png "sim splash")

with the simulated events the theoretical angular distribution was reproduced:

![Alt text](info/angular_distribution.pdf "sim ang")

## Compile

In order to compile this program you need the following library:

* ROOT 6
* Armadillo C++

then you can compile it using:

`./Compile`

## Usage

You can specify the number of events to be generated as follow:

`./sim *number_of_events_to_be_generated*`
