Hello!

I am working on a project where I am trying to create a timer using an Attiny microcontroller.
I do not mention the model name of the Attiny, because it is not set in stone due to that I have a couple of different configurations in mind.


Let me explain the main goals and requirements of this project (skip to "the hardware I need to include" if you want to, the questions are related to those):


* Create a timer in the form factor of a credit card, powered by batteries (my plan is CR2032/26)
* The timer will allow counting down from 1 minute - 120 minutes.
* The time left will be displayed by using 12 LEDs arranged in a progress bar pattern. As the time left decreases, the number of lit LEDs decrease.
The last LED that is on in the pattern will be blinking every second to indicate countdown.
See the image here <ADD IMAGE> for what I mean:
* The user should allow setting 3 timer presets (for example, if you often count down to 30 minutes, you should be able to save it and load it)
* The timeout should be indicated by sounding a buzzer.
* The purpose of the timer is to be used for focus work sessions (e.g. studying on the go), so it is going to allow to user to select the time they want to work, 
the time they want to take a break, and how many times that should be repeated.
    * A consequence of the just mentioned requirement is that I want to add 3 LEDs on top of the 12 LEDs to indicate if the user is "working", "resting", or "setting the time".


Software requirements:
* For learning purposes, I would implement the main parts of the program in AVR Assembly. (Feel free to ramble as much as you want about that, but I have experience with C and I want to challenge myself.) (the I2C timer interaction, I'm doin' that in C. Don't worry.)


Thus, the hardware I need to include:

* 5 buttons (2 for timer settings (+/- time), 3 preset buttons) 
* 15 LEDs (12 for countdown time, 3 for timer state display)
* Timer
* Buzzer to indicate timeout


So to my questions.

# Question 1: Charlieplexing timer

## Background

Assuming the buzzer can be driven by some kind of square wave output from my timer (for example MCP7940M offers a 1Hz square wave output), I would need:

5 button GPIOs + 15 LEDs + 2 timer I2C GPIOs + 1 timer interrupt pin = 23 pins

A method to control n(n-1) LEDs with n pins is Charlieplexing. I am considering Charlieplexing the 12 pin progressbar, requiring a 4-pin setup. You can read all about it here: https://circuitdigest.com/microcontroller-projects/charlieplexing-arduino-to-control-12-leds-with-4-gpio-pins

So, I have come up with a code that works on an Arduino UNO for what I am trying to do: 

## Actual question

My "Charlieplexing" tests require a delay of 1000us to lit all LEDs without them appearing blinking to the human eye.
I was considering using an external timer to generate 1000us interrupts, but I can't find such timer and I assume that would be too fast for the device to handle.
How bad is it to use the internal Attiny timer for such short periods? The specs hint that it is quite inaccurate. Any alternatives I should consider? If precise timing isn't used, since Charlieplexing requires turning on and off LEDs super fast each second so all LEDs appear lit to the human eye, the LEDs appear to have different brightness.

And yeah, Charlieplexing requires turning on and off LEDs super fast each second so all LEDs appear lit to the human eye. I assume this rapid on/off would draw more power than just having them on all the time?

# Question 2: Charlieplexing - other alternatives


I may considering changing to shift registers, they seem easier. Read question 1 for more context. Anyone have experience controlling 12 GPIOs with less than 12 pins (in my case Charlieplexing allows me to use 4)? What other alternatives should I consider? How much better is shift registers?


# Question 3: External timer choice

I am planning on counting down the timer by generating timer interrupts every second to an interrupt pin, making the device decrement a "time left" variable. I also need the timer to beep a buzzer every second once the timer has run out.

I am currently considering the MCP7940M timer: does anyone have experience? And it would cost as many pins to use I2C as to connect a crystal to the Attiny and use the internal timer, so I am considering doing that instead. Is it visable?

# Question 4: Button interrupts

I want each of the 5 buttons to generate an interrupt if you click them. I've heard that hardware interrupts can be a pain with buttons unless you take some precautions. I have done research for question 1-3, but I admit this one not that much. Is it viable to use hardware interrupts for buttons? What precautions should I take when designing the hardware? Should I consider polling instead? I think that interrupts will be cleaner and more power efficient, hence why I am using that.


Sorry if this was a lot of questions, I'd be happy to just get an answer to a few of them :)