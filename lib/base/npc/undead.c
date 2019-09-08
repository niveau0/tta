/* basic functions for undead */

#include <properties.h>

nomask void add_prop(mixed prop, mixed value);

private int tlevel;

static nomask void
init_undead()
{
    add_prop(LIVE_UNDEAD, 1);
    add_prop(LIVE_NEVER_BLEED, 1);
    add_prop(LIVE_NEVER_UNCONSCIOUS, 1);
}

static nomask void
set_turn_level(int lev)
{
    tlevel = lev;
}

nomask int
query_turn_level()
{
    return tlevel;
}

nomask int
turn_cmd(string arg)
{
}
