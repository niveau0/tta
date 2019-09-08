/*
 * weathermaster, handle all weatherchanges
 * supports weather prediction and changes by magic
 */

#include <access.h>
#include <macros.h>
#include <mudtime.h>
#include <properties.h>
#include <weather.h>

#define WORLD_XMIN -57024000 /* 900 miles */
#define WORLD_XMAX  31680000 /* 500 miles */
#define WORLD_YMIN -12672000 /* 200 miles */
#define WORLD_YMAX  76032000 /* 1200 miles */
#define MAX_CLOUD (63360*20) /* 20 miles max size */
#define MIN_CLOUD (63360*5)  /* 5 miles min size */
#define NUMBER_OF_CLOUDS 20

#define SAVEFILE "/syslog/weather"

int **pattern;

static int     wind, **clouds;
static mapping hour_events;

private int *make_clouds();

static nomask void
create()
{
    restore_object(SAVEFILE);
    if (!pattern)
	pattern = ({ });
    
    hour_events = ([ ]);
    make_clouds();
    call_out("sunrise", 10);
    call_out("calc_weather", 180);
}

/*
 * Function name: edit_pattern
 * Description:   start a simple editor to edit the weather patterns
 */
nomask void
edit_pattern()
{
    object tp;

    tp = this_player();
    if (!_ACCESS_WRITE(SAVEFILE, tp, "edit_pattern"))
	error("Illegal call to edit_pattern");

    tp->catch_tell("Weather pattern editor - Use '?' for help\n*", 1);
    input_to("edit_cmd", 0);
}

static nomask void
edit_cmd(string str)
{
    int    i, sz, *current;
    string out;
    object tp;

    tp = this_player();
    switch (str)
    {
      case 0:
      case "":
	  tp->catch_tell("*", 1);
	  break;
      case "?":
	  tp->catch_tell("Commands:\n" +
			 "patterns, table, edtable, edpattern, q\n*", 1);
      case "q":
	  tp->catch_tell("Saving data and leaving weather editor.\n");
	  save_object(SAVEFILE);
	  return;
      case "edpattern":
	  tp->catch_tell("Not implemented yet.\n*", 1);
	  break;
      case "edtable":
	  tp->catch_tell("Not implemented yet.\n*", 1);
	  break;
      case "patterns": /* show current pattern definitions */
	  if (!(sz=sizeof(pattern)))
	  {
	      tp->catch_tell("No patterns defined.\n*", 1);
	      break;
	  }
	  out = "There are the following weather definitions:\n" +
		"               Daytime              |                 " +
		"Night\n" +
		"Temperature |  Humidity |   Wind    | " +
		"Temperature |  Humidity |   Wind\n" +
		" Max | Min  | Max | Min | Max | Min | " +
		" Max | Min  | Max | Min | Max | Min \n" +
		"--------------------------------------" +
		"------------------------------------\n";
	  
	  for (i=0, sz=sizeof(pattern); i<sz; i++)
	  {
	      current = pattern[i];
	      out += sprintf("%5d | %6d | %5d | %5d | %5d | %5d\n",
			     current[0], current[1], current[2],
			     current[3], current[4], current[5]);
	  }
	  tp->catch_tell(out + "*", 1);
	  break;
      case "table": /* show pattern <-> climate/region table */
	  tp->catch_tell("No table defined.\n*", 1);
	  break;
    }
    input_to("edit_cmd", 0);
}

/*
 * Function name: notify_hour_event
 * Description:   add calling object to the event list, to get notifed that a
 *                certain is reached
 * Arguments:     func - function to call
 *                hour - daytime that should cause the event
 */
nomask void
notify_hour_event(string func, int hour)
{
    object ob;
    
    if (!(ob = previous_object()) || !func)
	return;
    if (!hour_events[hour])
	hour_events[hour] = ({ ob, func });
    else
	hour_events[hour] += ({ ob, func });
}

/*
 * Function name: query_moon
 * Description:   return the current moon phase
 * Returns:       integer
 */
nomask int
query_moon()
{
    switch (DAY)
    {
      case 17..20:
	  return MOON_PHASE_4;
      case 21..24:
	  return MOON_PHASE_3;
      case 25..28:
	  return MOON_PHASE_2;
      case 29..31:
	  return MOON_PHASE_1; /* no moon */
      case 1..4:
	  return MOON_PHASE_2;
      case 5..8:
	  return MOON_PHASE_3;
      case 9..13:
	  return MOON_PHASE_4;
      default:		      
	  return MOON_PHASE_5; /* full moon */
    }
}

/*
 * Function name: sunrise
 * Description:   give different descriptions about the current sunposition
 */
static nomask void
sunrise()
{
    int    i, h;
    string str;
    object c, e, *u;
    mixed  events;
    
    call_out("sunrise", 3600 / TIME_SCALE);
    
    h = HOUR;
    for (i=sizeof(events = hour_events[h])-2; i>=0;i-=2)
    {
	if (!events[i]) /* object pointer gone? */
	    hour_events[h] = events[..i-1] + events[i+2..];
	else
	    call_other(events[i], events[i+1], h);
    }
    if (!(i = sizeof(u = users())))
	return;

    switch (h)
    {
      case DAWN:
	  str = DAWNDESC;
	  break;
      case DAYTIME:
	  str = DAYDESC;
	  break;
      case FORENOON:
	  str = FORENOONDESC;
	  break;
      case MIDDAY:
	  str = MIDDAYDESC;
	  break;
      case AFTERNOON:
	  str = AFTERNOONDESC;
	  break;
      case DUSK:
	  str = DUSKDESC;
	  break;
      case NIGHTTIME:
	  str = NIGHTDESC;
	  break;
    }
    
    while (i--)
    {
	if (str && (e = E(c = u[i])) &&
	    !(e->query_type() & (TYPE_INSIDE | TYPE_UNDER_WATER)))
	    c->catch_msg(str, c, MSG_SEE);
    }
}

/*
 * Function name: make_clouds
 * Description:   create some cloud areas that drift over the world and
 *                rain sometimes
 * Returns:       array with cloud data
 *                ({ ({ x, y, xmax, ymax, humidity }),
 *                   ({ x, y, xmax, ymax, humidity }), ... })
 */
private int *
make_clouds()
{
    int i;

    clouds = allocate(NUMBER_OF_CLOUDS);
    for (i=NUMBER_OF_CLOUDS; i--;)
    {
	clouds[i] = ({ WORLD_XMIN + random(WORLD_XMAX - WORLD_XMIN),
		       WORLD_YMIN + random(WORLD_YMAX - WORLD_YMIN),
		       MIN_CLOUD + random(MAX_CLOUD - MIN_CLOUD),
		       MIN_CLOUD + random(MAX_CLOUD - MIN_CLOUD),
		       random(20) + 80 });
    }
}

/*
 * Function name: make_wind
 * Description:   compute new wind value
 * Arguments:     season - current season code
 * Returns:       string, wind description
 */
private string
make_wind(int season)
{
    int oldwind, windchange;
    
    oldwind = wind;
    windchange = random(10) - 5;
    
    if (season == SUMMER)
	windchange -= random(2);
    else if (season == AUTUMN)
	windchange += random(2);

    if (windchange < 0)
	wind -= (random(-windchange) + 1);
    else if (windchange > 0)
	wind += (random(windchange) + 1); 
    
    if (wind < 0)
	wind = 0;
    else if (wind > 12)
	wind = 12;
    
    if (wind > 2 && oldwind != wind && !random(2))
    {
	if (oldwind < wind)
	    return "The wind is getting stronger.";
	return "The wind is blowing weaker now.";
    }
    return WINDDESC[(wind + 1) / 2];
}

/*
 * Function name: make_temperature
 * Description:   compute new temperature value
 * Arguments:     season - current season code
 * Returns:       string temperature description
 */
private string
make_temperature(int season)
{
    return "";
}

/*
 * Function name: make_humidity
 * Description:   compute new humidity value, includes rain and fog handling
 * Arguments:     season - current season code
 * Returns:       string humidity description
 */
private string
make_humidity(int season)
{
    return "";
}

/*
 * Function name: calc_weather
 * Description:   main function, calculates and outputs new weather
 */
static void
calc_weather()
{
    int    i, season;
    string desc;
    object env, *u;

    call_out("calc_weather", random(600) + 600); /* 10 - 20 minutes */

    season = SEASON;

    desc = make_wind(season) +
	   make_temperature(season) +
	   make_humidity(season);

    desc = BS(desc, 77, 2)[2..] + "\n";
    for (i=sizeof(u = users()); i--;)
    {
    	if (!(env = E(u[i])) ||
	    (env->query_type() & (TYPE_INSIDE | TYPE_UNDER_WATER)))
    	    continue;
	u[i]->catch_tell(desc);
    }
}
