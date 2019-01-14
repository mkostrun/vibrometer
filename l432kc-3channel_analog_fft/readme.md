# vibrometer

*Vibrometer* is a mechanical vibration detection and recording device.
For detection it uses the ADXL335 sensor, which is an analog triple axis accelerometer.
The three analog signals from the sensor are sampled by the microcontroller,
and their spectrogram (modulational amplitude vs. frequency) calculated in
single precision, and at regular intervals printed on the serial port.

## Features:

- 3 x 1024 long buffer for storing 3-axes accelerometer data

- configurable sampling period of 1, 2, 4, 8, 16 and 32 seconds, or
equivalently, width of frequency bin of 1, 2, 4, 8, 16 or 32 Hz,
and max frequency of 512, 256, 128, 64, 32 or 16 Hz.

- the device has two states: *print* (fft data to serial port), or *quiet* (accepts
change in configuration). Serial port parameters are: the speed is 115,200 bps, then 8N1
for 8 data bits, no parity check and 1 stop bit.

- Commands for manipulating the device:

  All modulation amplitudes are scaled (and clipped) to

  - *u*, set max modulation amplitude to 1

  - *d*, set max modulation amplitude to 0.1

  - *c*, set max modulation amplitude to 0.01

  - *m*, set max modulation amplitude to 0.001

  then rounded to a 2-digit hex number. This is printed on the serial port. If the
  value is 0, this is printed as '_'. Following last non-zero entry for modulation
  amplitude 'X' is printed, meaning that the remaining amplitudes are all zero. 

  The sampling period is specified as follows:

  - *0*, 1 second,

  - *1*, 2 seconds,

  - *2*, 4 seconds,

  - *3*, 8 seconds,

  - *4*, 16 seconds,

  - *5*, 32 seconds.
  
  
  Other commands, in *quiet* mode:

  - *f*, print firmware revision

  - *l*, print labels for the quantities with their values, as in 'NAME=val'

  - *s*, do not print labels

  - *p*, go to printing mode.


  To stop printing and go to *quiet* mode:

  - *q*
