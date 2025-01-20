# Pomodoro Timer

## What is this?

This Project aims to create a portable Pomodoro timer that you can carry around with you - regardless if studying in the library, on the bus, or in the Comfort of your own home!

### Components and technology

#### Programming languages

The main part of this Project is programmed in AVR Assembly. Some parts are coded in C.

#### Main Components used

* Attiny84A

* DS3231 timer


#### Building the Circuit/assembling the PCB

For the full description on how to build the Project, I will at some Point (**TODO**) provide documentation about how to assemble your own Pomodoro timer.


## Development

Under the `code` folder you will find a solution which you can import into Microchip Studio (version 7).

You also need to add the files `i2cmaster.h` and `i2cmaster.S` which are from Peter Fleury's I2CMaster library (can be found [here](http://www.peterfleury.epizy.com/avr-software.html?i=1))

## Documentation

See the `docs` folder for Project documentation.