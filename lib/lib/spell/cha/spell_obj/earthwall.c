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
    set_name("earthwall");
    set_short("earthwall");
    set_pshort("earthwalls");
    add_prop(OBJ_MATERIAL, M_EARTH);
}

string
long()
{
    return "It's a " + (range[0] / 12) + "x" + (range[1] / 12) +
	" feet wide wall of muddy earth.\n" + ::long();
}

int
E_block(object tp)
{
    tp->catch_tell("An earthwall blocks your way.\n");
    return 1;
}
