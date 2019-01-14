
//
//
static(URL, URL_OPTS, _SER_N_TOC, _SER_N_TIMEOUT);

static(URL, URL_OPTS, _SER_N_TOC, _SER_N_TIMEOUT);

_SER_N_TOC = 30;        // use toc() timer No. 29
_SER_N_TIMEOUT = 3;  // how much to wait in seconds between the attempts

attr=<<>>;
attr.devname = "tty";
attr.id_vendor_id = "0483";
// attr.id_serial_short = "0673FF534854845187093325";
attr.id_serial_short = "066DFF534854845187093341";
CHIP_ADD = usbdevname(attr);
printf("Trying to talk to device at %s\n", CHIP_ADD);

URL_OPTS = <<>>;
URL_OPTS.data_format  = "8N1";
URL_OPTS.speed    = 115200;
// URL_OPTS.debug   = 2;
//URL_OPTS.flow_control = "xon|xoff";
// URL_OPTS.flow_control = "hardware";
// URL_OPTS.eol = "\n";
URL = "serial://" + CHIP_ADD;
// URL_OPTS.debug=1;
url = URL;
url_opts = URL_OPTS;

if (!open(URL,URL_OPTS))
{ sleep (3); }

stm32_stop = function( )
{
  while (!isempty(readm(URL)))
  {
    writem(URL, "q");
  }
  return 0;
};

stm32_start = function( )
{
  writem(URL, "p");
  return 0;
};

stm32_period = function( val )
{
  l2p = round(log(val) / log(2));
  writem(URL, num2str(l2p));
  return 0;
};

stm32_maxrange = function( val )
{
  if (val == "c")
  {
    writem(URL, "c");
  }
  if (val == "d")
  {
    writem(URL, "d");
  }
  if (val == "u")
  {
    writem(URL, "u");
  }
  return 0;
};


//
// we provide two functions for the main programme
//
stm32_getdata = function( )
{
  static(delta_f, max_amp);

  if (!exist(delta_f))
  {
    delta_f = 1;
  }
  if (!exist(max_amp))
  {
    max_amp = 1;
  }

  for (i in 1:3)
  {
    tic(_SER_N_TOC);
    r = readm(URL);
    while (toc(_SER_N_TOC)<_SER_N_TIMEOUT && isempty(r))
    { r = readm  (URL); }

    if(!isempty(r))
    { break; }
    r = [];
    sleep(0.1);
  }

  if(isempty(r))
  { return []; }

  a = find(ascii(r)==10);
  if (a.n == 1)
  { a = [0, a]; }
  i = lastc(a,2);
  x = substr(r,(i[1]+1):(i[2]-1));

  x = strsplt(x, ",");
  if(x.n < 2)
  {
    return <<>>;
  }

  rval = <<>>;
  rval.raw  = r;
  rval.ch  = strtod(x[1]);
  if (rval.ch == 1)
  {
    rval.temp = strtod(x[2]);
    rval.bat = strtod(x[3]);
    rval.period  = 2^(strtod(x[4]));
    delta_f = 1 ./ rval.period;
    max_amp = 10^(-strtod(x[5]));
  }
  rval.avg = strtod(x[x.n - 1])/10;
  data = zeros(1024,1);
  if (x[x.n]!="X")
  {
    i = 1;
    j = 0;
    lx = strlen(x[x.n]);
    while (i <= lx)
    {
      j++;
      c = substr(x[x.n],i);
      if (ascii(c) <= 20)
      { break; }
      if (c == "_")
      {
        i++;
        continue;
      }
      sf = "0x" + substr(x[x.n],i:(i+1));
      data[j] = strtod(sf)/255;
      i = i + 2;
    }
  }
  _f = [0:(length(data)-1)]' .* delta_f;
  rval.fft = [_f, max_amp * data];

  return rval;
};



