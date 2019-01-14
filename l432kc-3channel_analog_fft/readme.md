# vibrometer

*Vibrometer* is a mechanical vibration detection and recording device.
For detection it uses the ADXL335 sensor, which is an analog triple axis accelerometer.
The three analog signals from the sensor are sampled by the microcontroller,
and their spectrogram (modulational amplitude vs. frequency) calculated in
single precision, and at regular intervals printed on the serial port.

## Features:

- 3 x 1024 long buffer for storing 3-axes accelerometer data

- configurable sampling period of 1, 2, 4, 8, 16 and 32 seconds, or
equivalently, width of frequency bin of 1, 1/2, 1/4, 1/8, 1/16 or 1/32 Hz,
and max frequency of 512, 256, 128, 64, 32 or 16 Hz.

- serial port parameters are: the speed is 115,200 bps, then 8N1
for 8 data bits, no parity check and 1 stop bit.

- The device uses TIM1 to create timed interrupts that initiate ADC sampling
of channels with the board designations A1, A2 and A3.
These use an end-of-conversion (EOC) interrupt, which in turn invokes DMA to transfer data back
to the user. Every 1/3 of sampling time, the sampled data is transferred to a new
buffer and its FFT computed, scaled and printed.

  In addition, serial port routines have beed expanded so to use circular buffer and
Arduino-like serial port handler. See the file *stm32l4xx_hal_uart.c* and
its header *stm32l4xx_hal_uart.h*  in *Drivers/STM32L4xx_HAL_Driver*, Src and Inc directories, respectively.


- Commands for manipulating the device can be received by the device only if the
device is in *quiet* mode. The device samples the data continously, but only in
*print* mode it performs the fast fourier transform and prints the data on serial port..

  In *print* mode the device responds to these commands:

  - *s*, stop printing labels

  - *q*, stop printing and go to *quiet* mode


  In *quiet* mode the following can be configured:


  All modulation amplitudes are scaled (and clipped) to

  - *u*, set max modulation amplitude to 1

  - *d*, set max modulation amplitude to 0.1

  - *c*, set max modulation amplitude to 0.01

  - *m*, set max modulation amplitude to 0.001

  then rounded to a 2-digit hex number. This is printed on the serial port. If the
  value is 0, this is printed as '_'. Following last non-zero entry for modulation
  amplitude 'X' is printed, meaning that the remaining amplitudes are all zero. 


  The sampling period is specified as follows:

  - *0*, 1 second

  - *1*, 2 seconds

  - *2*, 4 seconds

  - *3*, 8 seconds

  - *4*, 16 seconds

  - *5*, 32 seconds.
  

  Other commands, in *quiet* mode:

  - *f*, print firmware revision

  - *l*, print labels for the quantities with their values, as in 'NAME=val'

  - *s*, do not print labels

  - *p*, go to printing mode, resume printing of the FFT modulation amplitudes.
