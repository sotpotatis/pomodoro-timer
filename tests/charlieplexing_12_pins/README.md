# Charlieplexing 12 pins

For controlling each of the LEDs that are used for indicating the time left on the timer without having to use 12 pins for 12 LEDs, I used a technique called Charlieplexing.

Below is an explanation of the different files in this directory:

* `code/charlieplexing_12_leds/charlieplexing_12_leds.ino`: Code to test Charlieplexing up to 12 LEDs, with the last LED blinking. Proof of concept, does not neccessarily have to run on an Attiny microcontroller.

* `schematics/Charlieplexing 12 LEDs.fzz`: Fritzing project to build Charlieplexing 12 LEDs. Has a schematic grouped by pin, however, the breadboard is not cleanly routed and all connections are not correct. I must have done something wrong since Fritzings routing did not cooperate with me. **The breadboard layout in this file is not buildable**.

* `schematics/Charlieplexing 12 LEDs - Cleaned up breadboard version.fzz`: See  `schematics/Charlieplexing 12 LEDs.fzz`. The difference is that here, I routed the connections manually, **so this file is the one who has what I think is a somewhat understandable and buildable 12 LED charlieplexing circuit**.

* `schematics/Charlieplexing 12 LEDs - Alternative version with classical schematics.fzz`: Fritzing project to build Charlieplexing 12 LEDs. Breadboard connections unrouted. The interesting tab here is the "Schematics" tab, where a classical 4-pin Charlieplexing schematic is drawn, in the typical way that these schematics are drawn (not grouped by pin as for the other `.fzz` files, which are more intuitive for breadboard or routing in my opinion.)