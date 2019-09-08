/*
 * this file holds _all_ command delay values
 */

#include <properties.h>

#define MAX_DELAY 20   /* The maximum delay a command will get */
#define TIME_STOPPER -1

static mapping cmddelay;
static int world_delay;

static nomask void
create()
{
    cmddelay = ([
	"accept":1,
	"aid":1,
	"analyse":1,
	"attach":1,
	     
	"backstab":1,
	"buy":1,

	"cancel":1,
	"cast":1,
	"channel":2,
	"close":1,
	"count":2,

	"d":1,
	"detach":1,
	"disarm":1,
	"dismount":1,
	"down":1,
	"drink":1,
	"drop":1,
	     
	"e":1,
	"east":1,
	"eat":1,
	"enter":1,
	"exa":1,
	"examine":1,
	"ext":1,
	"extinguish":1,

	"fill":1,
	     
	"get":1,
	"give":1,
	     
	"hide":1,
	     
	"light":1,
	"lock":1,

	"meditate":1,
	"mood":1,
	"mount":1,

	"n":1,
	"ne":1,
	"north":1,
	"northeast":1,
	"northwest":1,
	"nw":1,
	
	"open":1,
	
	"pick":2,
	"pull":1,
	"push":1,
	"put":1,
	     
	"read":1,
	"remove":1,
	"rest":1,
	"roll":1,
	"run":1,

	"s":1,
	"se":1,
	"search":1,
	"sell":1,
	"shoot":3,
	"sit":1,
	"sleep":1,
	"smash":1,
	"sneak":2,
	"south":1,
	"southeast":1,
	"southwest":1,
	"stand":1,
	"steal":1,
	"stop":1,
	"sw":1,
	     
	"teach":1,
	"throw":1,
	"turn":1,

	"u":1,
	"unlock":1,
	"unwield":1,
	"up":1,
	"use":1,
	     
	"w":1,
	"wake":1,
	"walk":1,
	"whisper":1,
	"wield":1,
	"wear":1,
	"west":1,
	"write":1,
	]);
}

public void
set_world_delay(int arg)
{
    if(arg > MAX_DELAY)
	arg = MAX_DELAY;
    world_delay = arg;
}

public int
query_world_delay()
{
    return world_delay;
}

nomask int
query_delay(string cmd, object player)
{
    if (!world_delay && player->query_prop(LIVE_HASTED))
	return cmddelay[cmd] / 2;
    if (!world_delay)
	return cmddelay[cmd];
    if (player->query_prop(LIVE_HASTED) == TIME_STOPPER)
	return cmddelay[cmd];
    return world_delay;
}
