// Page params
#set text(
  font: "Calibri",
  size: 11pt
)
#set heading(
  numbering: "1.1"
)
#set document(
  author: "sotpotatis",
  title: "Pomodoro timer documentation"
)

#align(horizon+center, [
  #text(25pt)[*Pomodoro Timer Documentation*] \
  #text(18pt)[sotpotatis]
])
#pagebreak()
#outline()
#pagebreak()

= Instruction manual
This instruction manual is intended for end users and covers none of the technical or constructional aspects of the project.


#lorem(20)
= Building your own version of the timer

This part of the instructions walks you through building your own timer.

#lorem(20)

== Setting up build tools and uploading code to the timer <uploadingCode>

== Calibrating your timer

The project relies on the built-in Attiny84A timer, which is known to be rather inaccurate. However, by calibrating the timer, you'll get a timer that is (in normal conditions) only a few seconds off. Studying for 60 minutes and 5 seconds instead of 60 minutes is no big deal.

#block(
  fill: color.hsv(187deg, 82%, 79%, 61%),
  radius: 2pt,
  outset: 4pt,
  [*Note:* The idea of how to calibrate the timer should be credited to user `avrcandies` on AVRFreaks, who mentioned it in https://www.avrfreaks.net/s/topic/a5CV40000002wfpMAA/t399784 (post 11). Thanks to the people on Pluggakuten who filled in some details I was unsure about: https://www.pluggakuten.se/trad/korrigeringsmetod-for-att-fa-battre-noggrannhet-varfor-fungerar-den/]
)
=== Uploading the timer calibration code

You can access timer calibration features by recompiling the code after having changed a few settings.

In `softwareConst.h`, make sure that the constants `RUN_TIMER_CALIBRATION` and `RUN_TIMER_TEST` have the following values:
```c
#define RUN_TIMER_CALIBRATION 1
#define RUN_TIMER_TEST 0
```
Now, compile the code and upload it to the microcontroller (see #ref(<uploadingCode>) for more help).

=== Performing timer calibration<timerCalibration>

Plug the project into a power source. You should see that one of the LEDs on the board lights up. Make sure you have a reliable time source (for example, a timer) at hand. Now, proceed as follows:

+ Click on any button connected to the Pomodoro timer. At the same time, start your reliable timer set to 5 minutes #footnote[If you'd like to change this value to a different number of minutes, modify the file `timerCalibration.c` in the source code.]. _A different_ LED than before should turn on the board.
+ Now, once your reliable timer goes off, click on any button on the Pomodoro timer again. _Make sure to do this as close to the time your real timer goes off for best results_. Again, _A different_ LED than before should turn on.
+ Congratulations, your timer should now be calibrated! Move on to #ref(<timerTesting>) and repeat calibration if neccesary.

=== Timer calibration testing<timerTesting>

Now it is time to test your timer calibration. 

A few notes:

- *Note* that it is normal to have to redo the calibration step (#ref(<timerCalibration>)) a few times until you get a desirable result.
- *Note* that you should perform this testing with different durations. How to set this is explained below. For example, test with 5 minutes, 60 minutes and 90 minutes.
- *Note* that you should also test the timer in different conditions. Consider testing the timer both in a cool room and in hot sun if those are conditions that you expect to be using it in.

In `softwareConst.h`, make sure that the constants `RUN_TIMER_CALIBRATION` and `RUN_TIMER_TEST` have the following values:
```c
#define RUN_TIMER_CALIBRATION 0
#define RUN_TIMER_TEST 1
```
Also, go to `timerTest.c`. Look for this line of code:
```c
uint8_t MINUTES_TO_COUNT_TO = 90; // No. of minutes to count to for this test
```
Change this line to set how long the timer will count down. It is advisable that you test different values, for example: 5 minutes, 60 minutes, and 90 minutes#footnote[I test with these because those are the typical durations of my study sessions and breaks.].

Compile the code again and upload it to the microcontroller (see also #ref(<uploadingCode>) for more help).

Now, connect your timer to power. This time around, _you should not_ see any visual indication on the front LEDs. Get a reliable timer source, for example a stopwatch.

+ Click on any of the buttons on the Pomodoro timer. At the same time, start your stopwatch. You should see _one_ of the LEDs on the timer turn on.
+ Come back after close to the time mentioned in `MINUTES_TO_COUNT_TO`. Wait around. Soon, you should see all LEDs on the timer lighting up. _As soon as this happens_, stop your stopwatch.
+ Compare your measured value to the number of minutes set in `MINUTES_TO_COUNT_TO`. A few seconds off is acceptable, but if the error is too large, consider redoing the calibration step. See #ref(<timerCalibration>).


= Technical documentation

== Modifying constants
The code contains two files with constants:
- `softwareConst.h`
- `hardwareConst.h`
The constants are further explained with code comments in these files.