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
    set_name("woodwall");
    set_short("woodwall");
    set_pshort("woodwalls");

    add_prop(OBJ_MATERIAL, M_OAK);
}

string
long()
{
    return "It's a " + (range[0] / 12) + "x" + (range[1] / 12) +
	" feet wide wall of strong wood.\n" + ::long();
}

int
E_block(object tp)
{
    tp->catch_tell("A wall of wood blocks your way.\n");
    return 1;
}
