#include "xpmdigits.h"

int counter;

static void
create()
{
    call_out("cleanup", 10);
}

static nomask void
cleanup()
{
    destruct_object();
}

nomask string
query_ctype()
{
    return "image/xpm";
}

nomask string
start(mapping args)
{
    int    i, j;
    string num, pic, *lines;

    restore_object("/syslog/wwwcounter");
    num = (string)56789;
    
    pic = ("/* XPM */\n" +
	   "static char *counter[] = {\n" +
	   "/* width height num_colors chars_per_pixel */\n" +
	   "\"    " + (XPMWIDTH*strlen(num)) +
	   "    " + XPMHEIGHT + "       15            1\",\n" +
	   "/* colors */\n" +
	   "\". c #ffffff\",\n" +
	   "\"# c #000000\",\n" +
	   "\"a c #003333\",\n" +
	   "\"b c #403333\",\n" +
	   "\"c c #406666\",\n" +
	   "\"d c #806666\",\n" +
	   "\"e c #809999\",\n" +
	   "\"f c #bf9966\",\n" +
	   "\"g c #bf9999\",\n" +
	   "\"h c #bfcc66\",\n" +
	   "\"i c #bfcc99\",\n" +
	   "\"j c #bfcccc\",\n" +
	   "\"k c #ff9999\",\n" +
	   "\"l c #ffcc99\",\n" +
	   "\"m c #ffcccc\",\n" +
	   "/* pixels */\n");

    lines = allocate(XPMHEIGHT);
    
    for (i=0; i<strlen(num); i++)
    {
	for (j=0; j<XPMHEIGHT; j++)
	{
	    if (!lines[j])
		lines[j] = "";
	    switch (num[i])
	    {
	      case '0':
		  lines[j] += XPM_ZERO[j*XPMWIDTH .. j*XPMWIDTH+XPMWIDTH-1];
		  break;
	      case '1':
		  lines[j] += XPM_ONE[j*XPMWIDTH..j*XPMWIDTH+XPMWIDTH-1];
		  break;	  
	      case '2':		  
		  lines[j] += XPM_TWO[j*XPMWIDTH..j*XPMWIDTH+XPMWIDTH-1];
		  break;	  
	      case '3':		  
		  lines[j] += XPM_THREE[j*XPMWIDTH..j*XPMWIDTH+XPMWIDTH-1];
		  break;	  
	      case '4':		  
		  lines[j] += XPM_FOUR[j*XPMWIDTH..j*XPMWIDTH+XPMWIDTH-1];
		  break;	  
	      case '5':		  
		  lines[j] += XPM_FIVE[j*XPMWIDTH..j*XPMWIDTH+XPMWIDTH-1];
		  break;	  
	      case '6':		  
		  lines[j] += XPM_SIX[j*XPMWIDTH..j*XPMWIDTH+XPMWIDTH-1];
		  break;	  
	      case '7':		  
		  lines[j] += XPM_SEVEN[j*XPMWIDTH..j*XPMWIDTH+XPMWIDTH-1];
		  break;	  
	      case '8':		  
		  lines[j] += XPM_EIGHT[j*XPMWIDTH..j*XPMWIDTH+XPMWIDTH-1];
		  break;	  
	      case '9':		  
		  lines[j] += XPM_NINE[j*XPMWIDTH..j*XPMWIDTH+XPMWIDTH-1];
		  break;
	    }
	}
    }
    pic += (implode(lines, "\n") +
	    "\nSIMPLE  =                    T\n" +
	    "BITPIX  =                    8\n"+
	    "NAXIS   =                    2\n"+
	    "NAXIS1  =                    " + (counter * XPMWIDTH) + 
	    "\nNAXIS2  =                    " + XPMHEIGHT +
	    "\nHISTORY Written by TTA\n" +
	    "END");

    return pic;
}
