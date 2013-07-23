# ATtiny based interactive sound device

## Overview

This code consists of two programs, each for a separate _Atmel AVR ATtiny_ microcontroller:

* _shaker_ implements a program that does these things:
 * it controls an LED using PWM (`led_fader.c`) and produces a "glowing" effect while the device is in "idle" mode (i.e. not shaken)
 * it analyzes voltages of an analog accelerometer and recognizes shocks that, when summed up, will be interpreted as a shake
 * on shaking it will fade the LED to full power and give a signal to a second ATtiny chip running the _sound_ program
* _sound_ implements a modified program named _Simple SD Audio Player with an 8-pin IC_ [_sd8p_ from elm-chan.org](http://elm-chan.org/works/sd8p/report.html)
 * it will receive the signal from the first ATtiny chip and play a sound file on an SD card
 * on every further signal during playback it will change to the next sound file
 * after playback it will go back to a power saving "idle" mode

## License

The program _shaker_ is released under BSD 3-clause license which is contained in the file `shaker/LICENSE`.

The program _sound_ is Copyright (C) 2010 by [ChaN](http://elm-chan.org/) and contains only small modifications by the author of the above program and is licensed under the conditions published in the source code.