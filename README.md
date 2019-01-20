# vibrometer


![image1](https://github.com/mkostrun/vibrometer/blob/master/art/lg-d1-201901120934-ch2.png?raw=true)

Fig. 1: Spectrogram of empty washing machine in speed wash which might have faulty
one of the dampers (shock absorbers).
At around 12.5 minutes the strong oscillations at low frequencies (banging)
force the controller to increase the spin frequency from 17 Hz to 20 Hz.
The spectral data was collected using the 3-channel analog accelerometer ADXL335
and the STM32L432KC (ARM Cortex M4) development board.


### Motivation

So I got this life-is-good front-load washing machine a few years ago. Recently it started making loud
noises during spin cycle. Finally, it would just refuse to start the spin cycle and instead
spend entire night trying to balance load and eventually stopping with an error message.
    Checking of internet resources suggested that the likely cause of the problem is that the
drum bearings were gone. Posted videos suggested that the repair would be straightforward
and fairly quick.
    So I ordered bearings from no-name vendor on brasilian river named web site and on the day
they were supposed to arrive started the work.

Eventually everything was put together with new bearings, but in the process of assembly I have
noticed that one damper got extended to its maximum length. I recall reading in the service
manual that if that happens the damper is gone but I had to use it because I did not have
spare in reserve.

This is where the story starts and is still going on strong.

#### Not all dampers are the same

I operated washing machine with the damaged damper, and it seemed that there
were excessive vibrations.
Not reading the service manual I ordered a damper that seemed right. When it arrived I installed
it and noticed even worse performance. Then I started noticing that not all the dampers
are the same.
Went back on line and from the garage bay site ordered set of dampers advertised as new.
When they arrived I put them in and noticed that by the dates stamped on the dampers
these are in fact very old dampers.
First one was total failure.

## Introduction

At that point I asked myself how can I quantify my experiences.
I found two solutions:

1. Some android phones have built in accelerometers. Mine can sample with rate up to 100 Hz,
and applications exist which allow data to be saved locally and exported.

2. Build a vibrometer using embedded system platform and use an analog accelerometer for
sensing. Analog sensing is important because it is simpler and low pass filtering, or
integration, can be added.

I did test the first one, and did not like it. The second one took me few days to build and
program and gave me amount of control over data acquisition that I preferred, so I went with it.

The following project provides three folders:

1. *art* - spectrograms collected under different circumstances using the sensor system.

2. *l432kc-3channel_analog_fft* - firmware for STM32L432KC that does the data collection,
3 channel FFT processing and prints modulation amplitudes to serial port.
Also provided are routines for interrupt printing with circular buffer over serial port.

3. *rlabplus* - scripts for demo, data recording and data analysis.

