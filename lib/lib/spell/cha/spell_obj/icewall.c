inherit "/base/wall";

#include <material.h>
#include <macros.h>
#include <living.h>
#include <skills.h>
#include <properties.h>

static void
create_wall()
{
    set_name("wall");
    set_name("icewall");
    set_short("icewall");
    set_pshort("icewalls");

    add_prop(OBJ_MATERIAL, M_ICE);
}

string
long()
{
    return "It's a " + (range[0] / 12) + "x" + (range[1] / 12) +
	" feet wide wall of ice.\n" + ::long();
}

int
E_block(object tp)
{
    tp->catch_tell("A wall of ice blocks your way.\n");
    return 1;
}
