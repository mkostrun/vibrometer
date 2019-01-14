//
//
//
saveplot=1;

ch_label = ["X (up/down)", "Y (left/right)", "Z (back/forth)"];

ddir = "./data/";

desc = "Empty Speed Wash (wash:cold/spin:medium/soil:light)";
session = "lg-d1-201901120934";
DT0 = 0;
DT = 16; DDT=1;

// desc = "Empty Speed Wash (wash:cold/spin:medium/soil:light)";
// session = "lg-d2-201901121137";
// DT0 = 0;
// DT = 17; DDT=1;

// desc = "Loaded Normal Wash (wash:cold/spin:medium/soil:normal)";
// session="lg-d2-loaded-201901121503";
// DT0 = 20;
// DT  = 55;
// DDT = 5;

for (ch in [1:3])
{
  fn = ddir + session + "-" + num2str(ch, "ch%.0f") + ".dat";
  x = readm(fn);
  t = x[2:x.nr;1];
  per = round(mean(diff(t)));
  t = t / 60; // seconds to minutes
  f = x[1;2:513];
  m = x[2:x.nr;2:513];
  log10m = ifelse(m>1e-5, log10(m), -5);

  // plot directly to 
  if (saveplot)
  {
    W = gnuwins(1, "eps");
    _pn = "./fig/" + session + "-" + num2str(ch, "ch%.0f");
    gnucmd("set output '" + _pn + ".eps'");
  }
  else
  {
    gnuwins (1);
  }
  gnuwin  (1);
  gnutitle  (desc + ": " + ch_label[ch] + " motion");
  gnulimits (DT0,DT,0,32);
  // _cmd=["clear",  "set pm3d map", "set cntrparam level 10",...
  //     "unset surface", "unset clabel", "set view 0,0", "unset ztics" ];
  gnucmd("set pm3d map");
  gnuxtics (DDT,1);
  gnuytics (4,4);
  gnuylabel("Frequency (Hz)");
  gnuxlabel("Time (min)");
  gnuzlabel("Log10(m)");
  gnusplot( << y=f; x=t; z=log10m >> );
  if (saveplot)
  {
    gnuclose(1);
//     system("(sleep 2; eps2pdf -f " + _pn + " 2>/dev/null 1>/dev/null)&");
    system("(sleep 2; eps2png -f " + _pn + " 2>/dev/null 1>/dev/null)&");
  }
}










