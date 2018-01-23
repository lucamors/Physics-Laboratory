# Three-Body (Photon) Decay Simulation Program

## Description

This is a simple decay simulation program that can generate three gamma decay from positronium.

![Alt text](info/splash.png "sim splash")

with the simulated events the theoretical angular distribution was reproduced:

<object data="info/angular_distribution.pdf" type="application/pdf" width="700px" height="700px">
    <embed src="info/angular_distribution.pdf">
        This browser does not support PDFs. Please download the PDF to view it: <a href="info/angular_distribution.pdf"></a>.</p>
    </embed>
</object>

## Compile

In order to compile this program you need the following library:

* ROOT 6
* Armadillo C++

then you can compile it using:

`./Compile`

## Usage

You can specify the number of events to be generated as follow:

`./sim *number_of_events_to_be_generated*`
