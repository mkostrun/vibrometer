# rlabplus scripts

This directory contains scripts for recording data from the sensor and creating
spectrograms using gnuplot.

The scripts are

1. *main1_demo.r3*  - collect data from sensor and plot the spectrograms for each
axis once a second, all on the same plot.

2. *main2_record.r3* - collect data from sensor, plot it, and save it to three
separate files *ch1*, *ch2* and *ch3*. Data is saved with windows-compatible
end-of-line "\r\n" and are comma separated. First line of the file are the
frequency bins with time stamp 0, while the rest of the lines start
with their time stamp and is then followed by 255 modulational amplitudes.
In the recording the mean value is neglected (should be ADC equivalent of 0 - why? - because
the washing machine is not moving as a result of all the shaking,
and it starts and stops from rest).

3. *main3_plotfft.r3* - load saved data from files *ch1*, *ch2* and *ch3*,
and create their spectrograms. Optional parameters are the starting and ending time
for the fft's as only the spin cycle is the most interesting one.

The filenames to which data are saved are timestamped with
the format "%Y%m%d%H%M", and prepended by the project name, here "lg",
and session, here "d1" or "d2" for two different dampers that were compared.

