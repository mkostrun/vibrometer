//
//
//
gnuwin(1);
XMAX = 32;
DX = 4;
if (rx == "u")
{
  YMAX = 1;
}
else if (rx == "d")
{
  YMAX = 0.1;
}
else if (rx == "c")
{
  YMAX = 0.01;
}
else if (rx == "m")
{
  YMAX = 0.001;
}
DY = YMAX/10;
xfrac = 0.82;

// big plot, takes the whole window
gnuwin (1);
gnulimits(0,XMAX,0,YMAX);
gnuxtics (DX,8);
gnuytics (DY,10);
gnulegend(["Sensor Signal:", ""] + members(data));
gnuylabel("Modulation Ratio");
gnuxlabel("Frequency (Hz)");
gnuformat([...
  "with lines lt 1 lw 3 lc @black@", ...
    "with lines lt 1 lw 3 lc @purple@", ...
        "with lines lt 1 lw 3 lc @red@", ...
[]]);
// gnutext  ("AVG = " + text(x.avg,"%.0f"), ...
//   [xfrac*XMAX,0.62], 0.66, "first" ,"tc @darkgrey@");
if (exist(x.min) && exist(x.max))
{
  gnutext  ( num2str(x.min,"(%.0f,") + num2str(x.max,"%.0f)"), ...
      [xfrac*XMAX+90,0.62], 0.66, "first" ,"tc @darkgrey@");
}

if (exist(savefile))
{
  gnuplot ( data, savefile);
}
else
{
  gnuplot ( data );
}

// stop()


