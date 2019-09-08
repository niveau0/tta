inherit "/base/container";

#include <material.h>
#include <stdproperties.h>

static void
create_container()
{
    set_name("pouch");
    add_name("leatherpouch");
    add_adj("soft");
    set_short("leatherpouch");
    set_pshort("leatherpouches");
    set_long("It's a piece of soft leather that forms a container. "+
	     "A leatherstripe goes through holes in the upper end and "+
	     "permits to close it, but not very tight.\n");
    set_lwvs(0, 250, 200, 9);
    set_max_weight(250 + 950+random(50) );
    set_max_volume(200 + 600+random(200));
    set_value(8);
    add_prop(OBJ_MATERIAL, M_LEATHER);
}
