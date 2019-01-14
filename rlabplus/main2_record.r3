//
//
//
rfile module_init_stm32
plotme = 1;
saveme = 1;

if (plotme)
{
  gnuwins (1);
}

// recording session
rx = "c";
chs = [1:3];
per = 4; // seconds, with fixed 1024 pts

if (saveme)
{
  session = "lg-d2-loaded";
  ddir = "./data/";
  ts = timestamp("%Y%m%d%H%M");
  chfns = ddir + session + "-" + ts + "-" + num2str(chs,"ch%.0f") + ".dat";

  if (saveme)
  {
    for (idx_ch in range(chfns))
    {
      fch = chfns[idx_ch];
      open(fch,"w");
      close(fch);
    }
  }
}

// configure:
stm32_stop();
stm32_period(per);
stm32_maxrange(rx);
stm32_start();

icount = 0;

tic(5);
data = <<>>;
while (1)
{
  sleep(0.05);
  x = stm32_getdata();

  if (isempty(x))
  {
    continue;
  }

  if (x.ch == 1)
  {
    icount ++;
  }

  if (saveme)
  {
    fch = chfns[x.ch];
    _d = x.fft';
    open(fch,"a");
    if (icount == 1)
    {
      writem(fch, [0,_d[1;]],<<csp=",";eol="\r\n";format=["%e"]>>);
    }
    writem(fch, [toc(5),_d[2;]],<<csp=",";eol="\r\n";format=["%.1f", "%e"]>>);
    close(fch);
  }

  spinner();

  if (plotme)
  {
    data.[x.ch] = x.fft;
    rfile module_print
  }
}




