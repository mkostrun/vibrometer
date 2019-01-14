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
