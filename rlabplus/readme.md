# rlabplus scripts

This directory contains scripts for recording data from the sensor and creating
spectrograms using gnuplot.

The scripts are

1. *main1_demo.r3*  - collect data from sensor and plot the spectrograms for each
axis once a second, all on the same plot.

2. *main2_record.r3* - collect data from sensor, plot it, and save it to three
separate files *ch1*, *ch2* and *ch3*.

3. *main3_plotfft.r3* - load saved data from files *ch1*, *ch2* and *ch3*,
and create their spectrograms. Optional parameters are the starting and ending time
for the fft's as only the spin cycle is the most interesting one.

The filenames to which data are saved are timestamped with
the format "%Y%m%d%H%M", and prepended by the project name, here "lg",
and session, here "d1" or "d2" for two different dampers that were compared.

