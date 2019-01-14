//
//
//
gnuwins (1);
rfile module_init_stm32

rx = "c";
    
// configure:
stm32_stop();
stm32_period(4);
stm32_maxrange(rx);
stm32_start();

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

  spinner();

  data.[x.ch] = x.fft;

  //
  rfile module_print
}




