inherit "/base/player";

#include <properties.h>

void
start_player(string n)
{
    add_prop(LIVE_SEE_INVIS, 100);
    ::start_player(n);
}
