/*
 * mudtime functions
 */
#include <language.h>
#include <mudtime.h>

static string *month_names, *weekdays;

static void
create()
{
    month_names = ({ "Afteryule", "Solmath", "Rethe", "Astron", "Thrimidge",
		     "Forelithe", "Afterlithe", "Wedmath", "Halimath",
		     "Winterfilth", "Blotmath", "Foreyule", "Yule", "Lithe" });
    weekdays = ({ "Saturday", "Sunday", "Monday", "Tuesday", "Wednesday",
		  "Thursday", "Friday" });
}

nomask string
conv_real_time(int t)
{
    int    i;
    string *ret;

    if (t < 0)
	return "unknown";
    
    ret = ({ });
    if (i = t / 31536000)
    {
	if (i == 1)
	    ret += ({ "1 year" });	    
	else
	    ret += ({ i + " years" });
    }
    if (i = (t % 31536000) / 2592000)
    {
	if (i == 1)
	    ret += ({ "1 month" });
	else
	    ret += ({ i + " months" });
    }
    if (i = (t % 2592000) / 86400)
    {
	if (i == 1)
	    ret += ({ "1 day" });
	else
	    ret += ({ i + " days" });
    }
    if (i = (t % 86400) / 3600)
    {
	if (i == 1)
	    ret += ({ "1 hour" });
	else
	    ret += ({ i + " hours" });
    }
    if (i = (t % 3600) / 60)
    {
	if (i == 1)
	    ret += ({ "1 minute" });
	else
	    ret += ({ i + " minutes" });
    }
    if (i = (t % 60))
    {
	if (i == 1)
	    ret += ({ "1 second" });
	else
	    ret += ({ i + " seconds" });
    }
    
    if ((i = sizeof(ret)) < 2)
	return ret[0];
    
    return implode(ret[0..i - 2], ", ") + " and " + ret[i - 1];
}

nomask int *
time_get_int(int mod)
{
    int    t, year, month, day, hour, minute, second, weekday;
    
    /*
     * 365 days/year, 24 h/day, 60 mins/h and 60 seconds/min,
     */

    /* 1.12.94 is beginning time count */
    t = time() - 786240000;
    t *= TIME_SCALE; 
    t += mod;
    
    /* starting year: 2400 */
    year = t / 31536000 + 2400;           /* 365 * 24 * 60 * 60 */
    day  = (t % 31536000) / 86400;        /* 24 * 60 * 60 */
    if (day == 0)
    {
	day = 2; /* 2. Yule */
	month = 12;
	weekday = 0;
    }
    else if (day == 364)
    {
	day = 1; /* 1. Yule */
	month = 12;
	weekday = 6;
    }
    else if (day == 181)
    {
	day = 1; /* 1. lithe */
	month = 13;
	weekday = 6;
    }
    else if (day == 182)
    {
	day = 0; /* Midyear's day */
	month = 0;
	weekday = -1;
    }
    else if (day == 183)
    {
	day = 2; /* 2. lithe */
	month = 13;
	weekday = 0;
    }
    else if (day < 181)
    {
	weekday = day % 7;
	day--;
	month = day / 30;
	day = (day % 30) + 1;
    }
    else
    {
	weekday = (day - 1) % 7;
	day -= 4;
	month = day / 30;
	day = (day % 30) + 1;
    }
    
    hour   = (t % 86400)    / 3600 ;  /* 60 * 60 */
    minute = (t % 3600)     / 60   ;  
    second = (t % 60);

    return ({ year, month, day, hour, minute, second, weekday });
}

nomask string
time_get_desc(int add_time)
{
    int    *arr;
    string ret;

    arr = time_get_int(0);

    if (add_time)
    {
	ret = ("It's the " + (arr[HOUR_POS]+1) + ". hour of ");
	if (arr[DAY_POS] == 0)
	    ret += "the Midyear's Day " + arr[YEAR_POS] + ".\n";
	else
	    ret += (weekdays[arr[WEEKDAY_POS]] + ", " +
		    month_names[arr[MONTH_POS]] + " " + arr[DAY_POS] +
		    ", " + arr[YEAR_POS] + ".\n");
	return ret;
    }
    if (arr[DAY_POS] == 0)
	return "It's Midyear's Day " + arr[YEAR_POS] + ".\n";
    return ("It's " + weekdays[arr[WEEKDAY_POS]] + ", " +
	    month_names[arr[MONTH_POS]] + " " + arr[DAY_POS] +
	    ", " + arr[YEAR_POS] + ".\n"); 
}

int
is_it_night()
{
    int h;
    
    if ((h = time_get_int(0)[HOUR_POS]) < DAWN || h > DUSK)
	return 1;          
    return 0;
}

/*
 * Function name: time_get_string
 * Description:   Make a sophisticated human-readable non-numerical
 *                description of how late it is.
 * Arguments:     arr - array as returned by TIMEARRAY
 * Returns:       a string 
 */
string
time_get_string(int *arr)
{
    int quarter, minute, hour;
    string exact, boa, hourname, daytime, quarterstr;
    
    minute = arr[MINUTE_POS] + 7;
    quarter =  minute / 15 % 4;
    
    hour = arr[HOUR_POS];
    if (quarter == 3 || minute > 59)
	hour = (hour + 1) % 24;

    minute = minute % 15 - 7;
    
    /* Now we are done with computing numbers. */

    if (hour == 0)
	hourname = " midnight";
    else if (hour == 12)
	hourname = " noon";
    else
    {
	hourname = " " + LANG_NUMW(hour % 12);
	if (quarter == 0)
	    hourname += " o'clock";
	
	if (hour < DAWN || hour > DUSK)
	    hourname += " in the night";
	else if (hour < 12)
	    hourname += " in the morning";
	else if (hour < 18)
	    hourname += " in the afternoon";
	else
	    hourname += " in the evening";
    }

    switch(quarter)
    {
      case 0:
	quarterstr = "";
	break;
      case 1:
	quarterstr = " a quarter past";
	break;
      case 2:
	quarterstr = " half past";
	break;
      case 3:
	quarterstr = " a quarter to";	
	break;
      default:
	quarterstr = " an undefined quarter near";
    }

    if (minute<0)
    {
	boa = " before";
	minute = -minute;
    }
    else
	boa = " after";

    if (minute == 0)
    {
	boa = "";
	exact = "";
    }
    else if (minute < 3)
    {
	boa = "";
	exact = " about";
    }
    else if (minute < 6)
	exact = " just";
    else
	exact = "";

    return "It's" + exact + boa + quarterstr + hourname + ".\n";
}

/*
 * Function name: season
 * Description:   gives the actual season
 * Returns:       integer
 */
nomask int
season()
{
    int month, day, *t;

    t = time_get_int(0);

    month = t[MONTH_POS];
    day   = t[DAY_POS];

    switch (month)
    {
      case 3..6:
	if (month == 3 && day < 21)
	    return WINTER;
	if (month == 6 && day < 21)
	    return SPRING;
	return SUMMER;
      default:
	if (month == 9 && day < 21)
	    return SUMMER;
	if (month == 12 && day < 21)
	    return WINTER;
	return AUTUMN;
    }
}
