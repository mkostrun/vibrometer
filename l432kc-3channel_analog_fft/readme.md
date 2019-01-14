# vibrometer

*Vibrometer* is a mechanical vibration detection and recording device.
For detection it uses the ADXL335 sensor, which is an analog triple axis accelerometer.
The three analog signals from the sensor are sampled by the microcontroller,
and their spectrogram (modulational amplitude vs. frequency) calculated in
single precision.
